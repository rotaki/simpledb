#pragma once

#include <istream>
#include <vector>

namespace simpledb {
  class push_back_reader {
  public:
    push_back_reader(std::istream &pIS);
    int read();
    void unread(const int &pCh);
  private:
    int mCurrentPos;
    std::vector<char> mV;
  };
}
