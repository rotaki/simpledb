#include "log/logmanager.hpp"

namespace smartdb {
  log_manager::log_manager(file_manager* pFileManager, const std::string &pLogFile):
    mFileManager(pFileManager), mLogFile(pLogFile)
  {
    int logSize = mFileManager->length(mLogFile);
    auto byteVec = std::make_shared<std::vector<char>>(mFileManager->block_size(), 0);
    //std::vector<char> byteVec(mFileManager->block_size(), 0);
    mLogPage = std::make_unique<page>(byteVec);
    if (logSize == 0) {
      mCurrentBlk = append_new_block();
    } else {
      mCurrentBlk = block_id(mLogFile, logSize-1);
      // mCurrentBlk = std::shared_ptr<block_id>(new block_id(mLogFile, logSize-1));
      mFileManager->read(mCurrentBlk, *mLogPage);
    }
  }
  
  void log_manager::flush(const int &pLSN) {
    std::unique_lock<std::mutex> lock(mMutex);
    if (pLSN >= mLastSavedLSN) {
      flush();
    }
  }

  int log_manager::append(const std::vector<char> &pLogRecord) {
    std::unique_lock<std::mutex> lock(mMutex); // only one thread should add record to page
    int boundary= mLogPage->get_int(0);
    int recSize = pLogRecord.size();
    int bytesNeeded = recSize + sizeof(int); // record itself and size of record

    if (boundary - bytesNeeded < static_cast<int>(sizeof(int))) {
      flush();
      mCurrentBlk = append_new_block(); // the page might be dirty but ok
      boundary = mLogPage->get_int(0);
    }

    int recPos = boundary - bytesNeeded;
    mLogPage->set_bytes(recPos, pLogRecord);
    mLogPage->set_int(0, recPos);
    mLatestLSN += 1;
    return mLatestLSN;
  }
  
  log_manager::log_iterator::log_iterator(file_manager* ppFileManager, const block_id &ppBlockId) : mmFileManager(ppFileManager), mmBlockId(ppBlockId) {
    
    //std::vector<char> byteVec(mmFileManager->block_size(), 0);
    auto byteVec = std::make_shared<std::vector<char>>(mmFileManager->block_size(), 0);
    mmPage = std::make_unique<page>(byteVec);
    // mmPage = std::shared_ptr<page>(new page(byteVec));
    move_to_block(mmBlockId);
  }

  bool log_manager::log_iterator::has_next() {
    return ((mmCurrentPos < mmFileManager->block_size()) || (mmBlockId.number() > 0));
  }

  std::vector<char> log_manager::log_iterator::next() {
    if (mmCurrentPos == mmFileManager->block_size()) {
      mmBlockId = block_id(mmBlockId.file_name(), mmBlockId.number()-1);
      // mmBlockId = std::shared_ptr<block_id>(new block_id(mmBlockId->file_name(), mmBlockId->number()-1));
      move_to_block(mmBlockId);
    }
    std::vector<char> rec = mmPage->get_bytes(mmCurrentPos);
    mmCurrentPos += sizeof(int) + rec.size();
    return rec;
  }

  void log_manager::log_iterator::move_to_block(const block_id &ppBlockId) {
    mmFileManager->read(ppBlockId, *mmPage);
    mmBoundary = mmPage->get_int(0);
    mmCurrentPos = mmBoundary;
  }
  
  log_manager::log_iterator log_manager::iterator() {
    flush();
    return log_iterator(mFileManager, mCurrentBlk);
  }
  
  block_id log_manager::append_new_block() {
    block_id blockId = mFileManager->append(mLogFile);
    mLogPage->set_int(0, mFileManager->block_size());
    mFileManager->write(blockId, *mLogPage);
    return blockId;
  };

  void log_manager::flush() {
    mFileManager->write(mCurrentBlk, *mLogPage);
    mLastSavedLSN = mLatestLSN;
  }
}
