#pragma once

#include <string>
#include <vector>

namespace smartdb {
  class page {
  public:
    page(const int &pBlockSize);
    
    page(std::vector<char> &pByteBuffer);

    int get_int(const int &pOffset) const;

    void set_int(const int &pOffset, const int &n);

    std::vector<char> get_bytes(const int &pOffset) const;

    void set_bytes(const int &pOffset, const std::vector<char> &pByteBuffer);

    std::string get_string(const int &pOffset) const;

    void set_string(const int &pOffset, const std::string &pString);

    std::vector<char>& contents();
    
  private:
    std::vector<char> mByteBuffer;
  };
}

// todo
// default size
