#include <iostream>
#include <stdexcept>
#include <cstring>
#include <vector>

#include "file/page.hpp"
#include "utils/logger.hpp"

namespace smartdb {
  page::page(const int &pBlockSize) {
    mByteBuffer.resize(pBlockSize);
  }
  
  page::page(std::vector<char> &pByteBuffer) {
    mByteBuffer = pByteBuffer;
  }
  
  int page::get_int(const int &pOffset) const {
    if (pOffset + sizeof(int) > mByteBuffer.size()) {
      LOG_ERROR("offset: %d, sizeof(int): %ld, PAGE_SIZE:%lu", pOffset, sizeof(int), mByteBuffer.size());
      throw std::runtime_error("Page overflow while get_int");
    }
    int num;
    std::memcpy(&num, &mByteBuffer[pOffset], sizeof(int));
    return num;
  }

  void page::set_int(const int &pOffset, const int &num) {
    if (pOffset + sizeof(int) > mByteBuffer.size()) {
      LOG_ERROR("offset: %d, sizeof(int): %ld, PAGE_SIZE:%lu", pOffset, sizeof(int), mByteBuffer.size());
      throw std::runtime_error("Page overflow while set_int");
    }
    std::memcpy(&mByteBuffer[pOffset], &num, sizeof(int));
    return;
  }
  
  std::vector<char> page::get_bytes(const int &pOffset) const {
    int size = get_int(pOffset);
    if (pOffset + sizeof(int) + size > mByteBuffer.size()) {
      throw std::runtime_error("Page overflow while get_bytes");
    }
    std::vector<char> byteVec;
    byteVec.insert(byteVec.end(), &mByteBuffer[pOffset+sizeof(int)], &mByteBuffer[pOffset+sizeof(int)+size]);
    return byteVec;
  }

  void page::set_bytes(const int &pOffset, const std::vector<char> &pByteBuffer) {
    int size = pByteBuffer.size();
    if (pOffset + sizeof(int) + size > mByteBuffer.size()) {
      throw std::runtime_error("Page overflow whlile set_bytes");
    }
    set_int(pOffset, size);
    std::memcpy(&mByteBuffer[pOffset+sizeof(int)], &pByteBuffer[0], size);
  }

  std::string page::get_string(const int &pOffset) const {
    std::vector<char> byteVec = get_bytes(pOffset);
    std::string s(byteVec.begin(), byteVec.end());
    return s;
  }

  void page::set_string(const int &pOffset, const std::string &pString) {
    std::vector<char> v(pString.begin(), pString.end());
    set_bytes(pOffset, v);
  }

  int page::max_length(int pStrLen) {
    return sizeof(int) + pStrLen;
  }
  
  std::vector<char>& page::contents() {
    return mByteBuffer;
  }
}
