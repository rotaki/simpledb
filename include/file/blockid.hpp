#pragma once

#include <string>

namespace smartdb {
  class block_id {
    friend bool operator==(const block_id &pLhs, const block_id &pRhs);
    friend bool operator!=(const block_id &pLhs, const block_id &pRhs);
    friend bool operator<(const block_id &pLhs, const block_id &pRhs);
    friend bool operator>(const block_id &pLhs, const block_id &pRhs);
    friend bool operator<=(const block_id &pLhs, const block_id &pRhs);
    friend bool operator>=(const block_id &pLhs, const block_id &pRhs);
    
  public:
    block_id();
    block_id(const block_id &pBlk);
    block_id(const std::string &pFileName, const int &pBlockNum);
    block_id &operator=(const block_id &pBlk);

    bool is_null();

    std::string file_name() const;

    int number() const;

    bool equals(const block_id &obj) const;

    std::string to_string() const;

  private:
    std::string mFileName;
    int mBlockNum;
  };
}
