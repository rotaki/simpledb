#include "metadata/tablemanager.hpp"
#include "record/tablescan.hpp"

namespace smartdb {
  table_manager::table_manager(bool pIsNew, std::shared_ptr<transaction> pTx) {
    std::shared_ptr<schema> tCatSchema(new schema);
    tCatSchema->add_string_field("tblname", mMaxName);
    tCatSchema->add_int_field("slotsize");
    mTCatLayout = std::shared_ptr<layout>(new layout(tCatSchema));
    
    std::shared_ptr<schema> fCatSchema(new schema);
    fCatSchema->add_string_field("tblname", mMaxName);
    fCatSchema->add_string_field("fldname", mMaxName);
    fCatSchema->add_int_field("type");
    fCatSchema->add_int_field("length");
    fCatSchema->add_int_field("offset");
    mFCatLayout = std::shared_ptr<layout>(new layout(fCatSchema));

    if (pIsNew) {
      create_table("tblcat", tCatSchema, pTx);
      create_table("fldcat", fCatSchema, pTx);
    }
  }

  void table_manager::create_table(const std::string &pTblName, std::shared_ptr<schema> pSch, std::shared_ptr<transaction> pTx) {
    std::shared_ptr<layout> lt(new layout(pSch));
    
    // insert one record into tblcat
    std::shared_ptr<table_scan> tCat(new table_scan(pTx, "tblcat", mTCatLayout));
    tCat->insert();
    tCat->set_string("tblname", pTblName);
    tCat->set_int("slotsize", lt->slot_size());
    tCat->close();

    // insert a record into fldcat for each field
    std::shared_ptr<table_scan> fCat(new table_scan(pTx, "fldcat", mFCatLayout));
    for (const std::string &fldName: pSch->fields()) {
      fCat->insert();
      fCat->set_string("tblname", pTblName);
      fCat->set_string("fldname", fldName);
      fCat->set_int("type", pSch->type(fldName));
      fCat->set_int("length", pSch->length(fldName));
      fCat->set_int("offset", lt->offset(fldName));
    }
    fCat->close();
  }

  std::shared_ptr<layout> table_manager::get_layout(const std::string &pTblName, std::shared_ptr<transaction> pTx) {
    int size = -1;
    std::shared_ptr<table_scan> tCat(new table_scan(pTx, "tblcat", mTCatLayout));
    while (tCat->next()) {
      if (tCat->get_string("tblname") == pTblName) {
        size = tCat->get_int("slotsize");
        break;
      }
    }
    tCat->close();
    
    std::shared_ptr<schema> sch(new schema);
    std::map<std::string, int> offsets;
    std::shared_ptr<table_scan> fCat(new table_scan(pTx, "fldcat", mFCatLayout));
    while (fCat->next()) {
      if (fCat->get_string("tblname") == pTblName) {
        std::string fldName = fCat->get_string("fldname");
        int fldType = fCat->get_int("type");
        int fldLen = fCat->get_int("length");
        int offset = fCat->get_int("offset");
        offsets[fldName] = offset;
        sch->add_field(fldName, fldType, fldLen);
      }
    }
    fCat->close();
    return std::shared_ptr<layout>(new layout(sch, offsets, size));
  }
}
