#pragma once

#include <memory>

#include "log/logmanager.hpp"
#include "buffer/buffermanager.hpp"


namespace smartdb {
  class transaction;

  class recovery_manager {
  public:
    recovery_manager(transaction* pTx,
                     const int &pTxNum,
                     log_manager* pLM,
                     buffer_manager* pBM);

    void commit();

    void rollback();

    void recover();

    int set_int(buffer* pBuff, const int &pOffset, const int &pNewVal);

    int set_string(buffer* pBuff,
                   const int &pOffset, const std::string &pNewVal);


  private:
    transaction* mTx;
    int mTxNum;
    log_manager* mLM;
    buffer_manager* mBM;

    void do_rollback();
    void do_recover();
  };
}
