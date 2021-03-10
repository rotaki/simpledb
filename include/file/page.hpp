#pragma once

#include <memory>
#include <string>
#include <vector>

namespace simpledb {
class page {
public:
  page(int pBlockSize);

  page(const std::shared_ptr<std::vector<char>> &pByteBuffer);

  int get_int(int pOffset) const;

  void set_int(int pOffset, int n);

  std::vector<char> get_bytes(int pOffset) const;

  void set_bytes(int pOffset, const std::vector<char> &pByteBuffer);

  std::string get_string(int pOffset) const;

  void set_string(int pOffset, const std::string &pString);

  static int max_length(int pStrLen);

  std::shared_ptr<std::vector<char>>
  contents(); // sometimes page contents should be alive after the death of page
              // instance　(ここがただのポインタを返してしまうと,
              // pageが死んだあとnullになる)

private:
  std::shared_ptr<std::vector<char>>
      mByteBuffer; // ここがただのポインタだと開放するのを忘れてしまう恐れがあり.
                   // コンストラクタでもできるので.
};
} // namespace simpledb
