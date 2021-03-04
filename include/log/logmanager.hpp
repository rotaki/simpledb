#pragma once

#include <memory>

#include "file/filemanager.hpp"

namespace smartdb {
  class log_manager {
  public:
    log_manager(std::shared_ptr<file_manager> pFileManager, const std::string &pLogFile);
    int append(const std::vector<char> &pLogRecord);
    void flush(const int &pLSN);

    class log_iterator {
    public:
      log_iterator(std::shared_ptr<file_manager> ppFileManager, std::shared_ptr<block_id> ppBlockId);
      bool has_next();
      std::vector<char> next();
      void move_to_block(std::shared_ptr<block_id> ppBlockId);
    private:
      std::shared_ptr<file_manager> mmFileManager;
      std::shared_ptr<block_id> mmBlockId;
      std::shared_ptr<page> mmPage;
      int mmCurrentPos;
      int mmBoundary;
    };

    log_iterator iterator();

  private:
    std::shared_ptr<file_manager> mFileManager;
    std::shared_ptr<page> mLogPage;
    std::shared_ptr<block_id> mCurrentBlk;
    std::string mLogFile;
    int mLatestLSN = 0;
    int mLastSavedLSN = 0;
    std::mutex mMutex;
    
    std::shared_ptr<block_id> append_new_block();
    void flush();

    
  };
  
}
