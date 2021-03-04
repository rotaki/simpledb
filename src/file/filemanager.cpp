#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils/logger.hpp"
#include "file/filemanager.hpp"

namespace smartdb {
  file_manager::file_manager(const std::filesystem::path &pDbDirectory, const int &pBlockSize):
    mDbDirectory(pDbDirectory), mBlockSize(pBlockSize)
  {
    std::filesystem::directory_entry dir(mDbDirectory);
    mIsNew = !dir.exists();
    if (mIsNew) {
      std::filesystem::create_directory(mDbDirectory);
    }
    for (auto& p: std::filesystem::directory_iterator(mDbDirectory)) {
      if (0 == p.path().filename().string().find("temp")) {
        std::filesystem::remove(p.path());
      }
    }
  }
  void file_manager::read(const block_id &pBlockId, page &pPage) {
    std::unique_lock<std::mutex> lock(mMutex);
    
    const std::string fileName = pBlockId.file_name();
    std::shared_ptr<std::fstream> fileIO = get_file(fileName);
    size_t offset = pBlockId.number() * mBlockSize;
    
    // if (static_cast<int>(offset) >= get_file_size(fileName)) {
    //   std::cerr << "Reading past of file. Appending empty blocks." << std::endl;
    //   while (static_cast<int>(offset) >= get_file_size(fileName)) {
    //     append(fileName);
    //   }
    // }
    
    fileIO->seekp(offset, std::ios::beg);
    fileIO->read(&(pPage.contents()[0]), mBlockSize);
    if (fileIO->bad()) {
      throw std::runtime_error("I/O error while reading");
    }
    int readCount = fileIO->gcount();
    if (readCount < mBlockSize) {
      std::cerr << "Read less than a page" << std::endl;
      fileIO->clear();
      memset(&(pPage.contents()[readCount]), 0, mBlockSize-readCount);
    }
  }

  void file_manager::write(const block_id &pBlockId, page &pPage) {
    std::unique_lock<std::mutex> lock(mMutex);
    
    const std::string fileName = pBlockId.file_name();
    
    std::shared_ptr<std::fstream> fileIO = get_file(fileName);
    
    size_t offset = pBlockId.number() * mBlockSize;
    fileIO->seekp(offset, std::ios::beg);
    fileIO->write(&(pPage.contents()[0]), mBlockSize);
    if (fileIO->bad()) {
      throw std::runtime_error("I/O error while write");
    }
    fileIO->flush();
  }

  block_id file_manager::append(const std::string &pFileName) {
    std::unique_lock<std::mutex> lock(mMutex);
    std::shared_ptr<std::fstream> fileIO = get_file(pFileName);

    int newBlockNum = length(pFileName);
    block_id blockId(pFileName, newBlockNum);
    
    std::vector<char> byteVec;
    byteVec.resize(mBlockSize);
    
    fileIO->seekp(newBlockNum*mBlockSize, std::ios::beg);
    fileIO->write(&byteVec[0], mBlockSize);
    if (fileIO->bad()) {
      throw std::runtime_error("I/O error while append");
    }
    fileIO->flush();
    return blockId;
  }

  bool file_manager::is_new() {
    return mIsNew;
  }

  int file_manager::block_size() {
    return mBlockSize;
  }
  
  std::shared_ptr<std::fstream> file_manager::get_file(const std::string &pFileName) {
    std::shared_ptr<std::fstream> fileIO(new std::fstream);
    std::filesystem::path path = mDbDirectory / pFileName;
    
    if (mOpenFiles.find(path) != mOpenFiles.end()) {
      fileIO = mOpenFiles[path];
      if (fileIO->is_open()) return fileIO;
    }

    fileIO->open(path.string(), std::ios::binary | std::ios::in | std::ios::out);
    if (!fileIO->is_open()) {
      // clear the error flag
      fileIO->clear();
      // create a new file
      fileIO->open(path.string(), std::ios::binary | std::ios::trunc | std::ios::in | std::ios::out);
      fileIO->close();
      // reopen with original mode
      fileIO->open(path.string(), std::ios::binary | std::ios::in | std::ios::out);
      if (!fileIO->is_open()) {
        LOG_ERROR("can't open file: %s", pFileName.c_str());
        throw std::runtime_error("can't open file");
      }
    }

    mOpenFiles[path] = fileIO;
    return fileIO;
  }

  int file_manager::get_file_size(const std::string &pFileName)  {
    std::filesystem::path path = mDbDirectory / pFileName;
    int size;
    try {
      size = static_cast<int>(std::filesystem::file_size(path));
    } catch (std::exception &e) {
      size = 0;
    }
    return size;
  }

  int file_manager::length(const std::string &pFileName) {
    return get_file_size(pFileName) / mBlockSize;
  }

  
}
