#pragma once

#include <memory>

#include "logmanager.hpp"
#include "buffermanager.hpp"


namespace smartdb {
  class transaction;

  class recovery_manager {
  public:
    recovery_manager(std::shared_ptr<transaction> pTx,
                     const int &pTxNum,
                     std::shared_ptr<log_manager> pLM,
                     std::shared_ptr<buffer_manager> pBM);

    void commit();

    void rollback();

    void recover();

    int set_int(std::shared_ptr<buffer> pBuff, const int &pOffset, const int &pNewVal);

    int set_string(std::shared_ptr<buffer> pBuff,
                   const int &pOffset, const std::string &pNewVal);


  private:
    std::shared_ptr<transaction> mTx;
    int mTxNum;
    std::shared_ptr<log_manager> mLM;
    std::shared_ptr<buffer_manager> mBM;

    void do_rollback();
    void do_recover();
  };
}
