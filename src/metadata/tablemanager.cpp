#include "metadata/tablemanager.hpp"
#include "record/tablescan.hpp"

namespace smartdb {
  table_manager::table_manager(bool pIsNew, transaction* pTx) {
    schema tCatSchema;
    tCatSchema.add_string_field("tblname", mMaxName);
    tCatSchema.add_int_field("slotsize");
    mTCatLayout = layout(tCatSchema);
    
    schema fCatSchema;
    fCatSchema.add_string_field("tblname", mMaxName);
    fCatSchema.add_string_field("fldname", mMaxName);
    fCatSchema.add_int_field("type");
    fCatSchema.add_int_field("length");
    fCatSchema.add_int_field("offset");
    mFCatLayout = layout(fCatSchema);

    if (pIsNew) {
      create_table("tblcat", tCatSchema, pTx);
      create_table("fldcat", fCatSchema, pTx);
    }
  }

  void table_manager::create_table(const std::string &pTblName, const schema &pSch, transaction* pTx) {
    layout lt(pSch);
    
    // insert one record into tblcat
    table_scan tCat(pTx, "tblcat", mTCatLayout);
    tCat.insert();
    tCat.set_string("tblname", pTblName);
    tCat.set_int("slotsize", lt.slot_size());
    tCat.close();

    // insert a record into fldcat for each field
    table_scan fCat(pTx, "fldcat", mFCatLayout);
    for (const std::string &fldName: pSch.fields()) {
      fCat.insert();
      fCat.set_string("tblname", pTblName);
      fCat.set_string("fldname", fldName);
      fCat.set_int("type", pSch.type(fldName));
      fCat.set_int("length", pSch.length(fldName));
      fCat.set_int("offset", lt.offset(fldName));
    }
    fCat.close();
  }

  layout table_manager::get_layout(const std::string &pTblName, transaction* pTx) {
    int size = -1;
    
    table_scan tCat(pTx, "tblcat", mTCatLayout);
    while (tCat.next()) {
      if (tCat.get_string("tblname") == pTblName) {
        size = tCat.get_int("slotsize");
        break;
      }
    }
    tCat.close();
    
    schema sch;
    std::map<std::string, int> offsets;
    table_scan fCat(pTx, "fldcat", mFCatLayout);
    while (fCat.next()) {
      if (fCat.get_string("tblname") == pTblName) {
        std::string fldName = fCat.get_string("fldname");
        int fldType = fCat.get_int("type");
        int fldLen = fCat.get_int("length");
        int offset = fCat.get_int("offset");
        offsets[fldName] = offset;
        sch.add_field(fldName, fldType, fldLen);
      }
    }
    fCat.close();
    
    return layout(sch, offsets, size);
  }
}
