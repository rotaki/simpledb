#pragma once

#include <memory>

#include "file/filemanager.hpp"

namespace simpledb {
class log_manager {
public:
  log_manager(file_manager *pFileManager, const std::string &pLogFile);
  int append(const std::vector<char> &pLogRecord);
  void flush(const int &pLSN);

  class log_iterator {
  public:
    log_iterator(file_manager *ppFileManager, const block_id &ppBlockId);
    bool has_next();
    std::vector<char> next();
    void move_to_block(const block_id &ppBlockId);

  private:
    file_manager *mmFileManager;
    block_id mmBlockId;
    std::unique_ptr<page> mmPage;
    int mmCurrentPos;
    int mmBoundary;
  };

  log_iterator iterator();

private:
  file_manager *mFileManager;
  std::unique_ptr<page> mLogPage;
  block_id mCurrentBlk;
  std::string mLogFile;
  int mLatestLSN = 0;
  int mLastSavedLSN = 0;
  std::mutex mMutex;

  block_id append_new_block();
  void flush();
};

} // namespace simpledb
