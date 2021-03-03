#include <map>
#include <iostream>

#include "indexupdateplanner.hpp"
#include "tableplan.hpp"
#include "updatescan.hpp"
#include "indexmanager.hpp"
#include "selectplan.hpp"

namespace smartdb {
  index_update_planner::index_update_planner(std::shared_ptr<metadata_manager> pMM):
    mMM(pMM) {}

  int index_update_planner::execute_insert(std::shared_ptr<insert_data> pID, std::shared_ptr<transaction> pTx) {
    std::string tblName = pID->table_name();
    std::shared_ptr<plan> p = std::static_pointer_cast<plan>(std::shared_ptr<table_plan>(new table_plan(pTx, tblName, mMM)));

    // first insert the record
    std::shared_ptr<update_scan> s = std::dynamic_pointer_cast<update_scan>(p->open());
    if (!s) {
      throw std::runtime_error("type conversion failed");
    }
    s->insert();
    rid r = s->get_rid();

    // then modify each field, inserting index records
    std::map<std::string, std::shared_ptr<index_info>> indexes = mMM->get_index_info(tblName, pTx);
    std::vector<std::string> fields = pID->fields();
    std::vector<constant> vals = pID->vals();
    for (int i = 0; i < static_cast<int>(fields.size()); i++) {
      std::string fldName = fields[i];
      constant val = vals[i];
      std::cout << "Modify field " + fldName + " to val " + val.to_string() << std::endl;
      s->set_val(fldName, val);

      if (indexes.find(fldName) != indexes.end()) {
        std::shared_ptr<index> idx = indexes[fldName]->open();
        idx->insert(val, r);
        idx->close();
      }
    }
    s->close();
    return 1;
  }

  int index_update_planner::execute_delete(std::shared_ptr<delete_data> pDD, std::shared_ptr<transaction> pTx) {
    std::string tblName = pDD->table_name();
    std::shared_ptr<plan> p = std::static_pointer_cast<plan>(std::shared_ptr<table_plan>(new table_plan(pTx, tblName, mMM)));
    p = std::static_pointer_cast<plan>(std::shared_ptr<select_plan>(new select_plan(p, pDD->pred())));
    std::map<std::string, std::shared_ptr<index_info>> indexes = mMM->get_index_info(tblName, pTx);
    std::shared_ptr<update_scan> s = std::dynamic_pointer_cast<update_scan>(p->open());
    if (!s) {
      throw std::runtime_error("type conversion failed");
    }
    int count = 0;
    while (s->next()) {
      // first, delete the record's RID from every idnex
      rid r = s->get_rid();
      for (const auto& [fldName, iI]: indexes) {
        constant val = s->get_val(fldName);
        std::shared_ptr<index> idx = iI->open();
        idx->remove(val, r);
        idx->close();
      }

      // then delete the record
      s->remove();
      count++;
    }
    s->close();
    return count;
  }

  int index_update_planner::execute_modify(std::shared_ptr<modify_data> pMD, std::shared_ptr<transaction> pTx) {
    std::string tblName = pMD->table_name();
    std::string fldName = pMD->target_field();
    std::shared_ptr<plan> p = std::static_pointer_cast<plan>(std::shared_ptr<table_plan>(new table_plan(pTx, tblName, mMM)));
    p = std::static_pointer_cast<plan>(std::shared_ptr<select_plan>(new select_plan(p, pMD->pred())));

    std::map<std::string, std::shared_ptr<index_info>> indexes = mMM->get_index_info(tblName, pTx);
    std::shared_ptr<index> idx = (indexes.find(fldName) == indexes.end() ? nullptr : indexes[fldName]->open());

    std::shared_ptr<update_scan> s = std::dynamic_pointer_cast<update_scan>(p->open());
    if (!s) {
      throw std::runtime_error("type conversion failed");
    }
    int count = 0;
    while (s->next()) {
      // first, update the record
      constant newVal = pMD->new_value().evaluate(s);
      constant oldVal = s->get_val(fldName);
      s->set_val(pMD->target_field(), newVal);

      // then update the appropriate index, if it exists
      if (idx) {
        rid r = s->get_rid();
        idx->remove(oldVal, r);
        idx->insert(newVal, r);
      }
      count++;
    }

    if (idx) {
      idx->close();
    }

    s->close();
    return count;
  }

  int index_update_planner::execute_create_table(std::shared_ptr<create_table_data> pCTD, std::shared_ptr<transaction> pTx) {
    mMM->create_table(pCTD->table_name(), pCTD->new_schema(), pTx);
    return 0;
  }

  int index_update_planner::execute_create_view(std::shared_ptr<create_view_data> pCVD, std::shared_ptr<transaction> pTx) {
    mMM->create_view(pCVD->view_name(), pCVD->view_def(), pTx);
    return 0;
  }

  int index_update_planner::execute_create_index(std::shared_ptr<create_index_data> pCID, std::shared_ptr<transaction> pTx) {
    mMM->create_index(pCID->index_name(), pCID->table_name(), pCID->field_name(), pTx);
    return 0;
  }
}
