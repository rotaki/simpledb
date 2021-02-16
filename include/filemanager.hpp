#pragma once

#include <memory>
#include <mutex>
#include <map>
#include <filesystem>

#include "page.hpp"
#include "blockid.hpp"

namespace smartdb {
  class file_manager {
  public:
    file_manager(const std::filesystem::path &pDbDirectory, const int &pBlockSize);
    void read(const block_id &pBlockId, page &pPage) ;
    void write(const block_id &pBlockId, page &pPage);
    block_id append(const std::string &pFileName);
    bool is_new();
    int block_size();
    int length(const std::string &pFileName);
  private:
    std::shared_ptr<std::fstream> get_file(const std::string &pFileName);
    int get_file_size(const std::string &pFileName);
    
    std::filesystem::path mDbDirectory;
    int mBlockSize;
    bool mIsNew;
    std::mutex mMutex;
    std::map<std::filesystem::path, std::shared_ptr<std::fstream>> mOpenFiles;

  };
}
