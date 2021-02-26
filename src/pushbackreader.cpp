#include <vector>

#include "pushbackreader.hpp"

namespace smartdb {
  push_back_reader::push_back_reader(std::istream &pIS):
    mCurrentPos(0)
  {
    // get length of input
    pIS.seekg(0, pIS.end);
    int length = pIS.tellg();
    pIS.seekg(0, pIS.beg);

    mV.resize(length);
    pIS.read(&(mV[0]), length);
  }

  int push_back_reader::read() {
    if (mCurrentPos == static_cast<int>(mV.size())) {
      return -1;
    }
    return static_cast<int>(mV[mCurrentPos++]);
  }

  void push_back_reader::unread(const int &pCh) {
    if (mCurrentPos == 0) {
      throw std::runtime_error("push back buffer is full");
    }
    mV[--mCurrentPos] = static_cast<char>(pCh);
  }
}
