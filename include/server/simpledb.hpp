#pragma once

#include "file/filemanager.hpp"
#include "log/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "tx/transaction.hpp"
#include "metadata/metadatamanager.hpp"
#include "plan/planner.hpp"

namespace simpledb {
  class simpledb {
  public:
    static int mBlockSize;
    static int mBufferSize;
    static std::string mLogFile;
    
    simpledb(const std::string &pDirname, const int &pBlockSize, const int &pBuffSize);
    simpledb(const std::string &pDirname);

    std::unique_ptr<transaction> new_tx();
    metadata_manager* md_mgr();
    planner* plnr();
    file_manager* file_mgr();
    log_manager* log_mgr();
    buffer_manager* buffer_mgr();
    
  private:
    std::unique_ptr<file_manager> mFM;
    std::unique_ptr<buffer_manager> mBM;
    std::unique_ptr<planner> mP;
    std::unique_ptr<log_manager> mLM;
    std::unique_ptr<metadata_manager> mMM;
  };
}
