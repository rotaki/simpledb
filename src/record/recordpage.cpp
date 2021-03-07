#include <iostream>

#include "record/recordpage.hpp"
#include "utils/logger.hpp"

namespace simpledb {
  record_page::record_page(transaction* pTx,
                           const block_id &pBlockId,
                           const layout &pLayout):
    mTx(pTx), mBlockId(pBlockId), mLayout(pLayout) {
    mTx->pin(mBlockId);
  }

  int record_page::get_int(const int &pSlot, const std::string &pFldName) {
    int fldPos = offset(pSlot) + mLayout.offset(pFldName);
    return mTx->get_int(mBlockId, fldPos);
  }

  std::string record_page::get_string(const int &pSlot, const std::string &pFldName) {
    int fldPos = offset(pSlot) + mLayout.offset(pFldName);
    return mTx->get_string(mBlockId, fldPos);
  }

  void record_page::set_int(const int &pSlot, const std::string &pFldName, const int &pVal) {
    int fldPos = offset(pSlot) + mLayout.offset(pFldName);
    return mTx->set_int(mBlockId, fldPos, pVal, true);
  }

  void record_page::set_string(const int &pSlot, const std::string &pFldName, const std::string &pVal) {
    int fldPos = offset(pSlot) + mLayout.offset(pFldName);
    return mTx->set_string(mBlockId, fldPos, pVal, true);
  }

  void record_page::remove(const int &pSlot) {
    set_flag(pSlot, empty);
  }

  void record_page::format() {
    int slot = 0;
    while (is_valid_slot(slot)) {
      mTx->set_int(mBlockId, offset(slot), empty, false);
      schema sch = mLayout.get_schema();
      for (const std::string &fldName: sch.fields()) {
        int fldPos = offset(slot) + mLayout.offset(fldName);
        if (sch.type(fldName) == schema::integer) {
          mTx->set_int(mBlockId, fldPos, 0, false);
        } else {
          mTx->set_string(mBlockId, fldPos, "", false);
        }
      }
      slot++;
    }
  }

  int record_page::next_after(const int &pSlot) {
    return search_after(pSlot, used);
  }
  
  int record_page::insert_after(const int &pSlot) {
    int newSlot = search_after(pSlot, empty);
    if (newSlot >= 0) {
      set_flag(newSlot, used);
    }
    return newSlot;
  }

  block_id record_page::block() const {
    return mBlockId;
  }

  void record_page::set_flag(const int &pSlot, const int &pFlag) {
    mTx->set_int(mBlockId, offset(pSlot), pFlag, true);
  }

  int record_page::search_after(const int &pSlot, const int &pFlag) {
    int slot = pSlot;
    slot++;
    while(is_valid_slot(slot)) {
      if (mTx->get_int(mBlockId, offset(slot)) == pFlag) {
        return slot;
      }
      slot++;
    }
    return -1;
  }

  bool record_page::is_valid_slot(const int &pSlot) const {
    return offset(pSlot+1) <= mTx->block_size();
  }

  int record_page::offset(const int &pSlot) const {
    int slotSize = mLayout.slot_size();
    int offset = pSlot * slotSize;
    if (offset < 0) {
      LOG_ERROR("slot num %d, slot size %d, offset %d", pSlot, slotSize, offset);
      throw std::runtime_error("negative offset");
    }
    return offset;
  }
}

