#pragma once

#include <memory>
#include <vector>
#include <string>

#include "file/blockid.hpp"
#include "file/page.hpp"
#include "log/logmanager.hpp"

namespace simpledb {

  class transaction;
  
  class log_record {
  public:
    enum operation {
                    checkpoint = 0,
                    start = 1,
                    commit = 2,
                    rollback = 3,
                    setint = 4,
                    setstring = 5
    };
    virtual ~log_record();
    virtual int op() = 0;
    virtual int tx_number() = 0;
    virtual std::string to_string() = 0;
    virtual void undo(transaction* pTx) = 0;
    static std::unique_ptr<log_record> create_log_record(const std::vector<char> &pByteVec);
  };

  class checkpoint_record: public log_record {
  public:
    checkpoint_record();
    int op() override;
    int tx_number() override;
    std::string to_string() override;
    void undo(transaction* pTx) override;
    static int write_to_log(log_manager* pLM);
  };

  class start_record: public log_record {
  public:
    start_record(page* pPage);
    int op() override;
    int tx_number() override;
    std::string to_string() override;
    void undo(transaction* pTx) override;
    static int write_to_log(log_manager* pLM,
                            const int &pTxNum);
  private:
    int mTxNum;
  };

  class commit_record: public log_record {
  public:
    commit_record(page* pPage);
    int op() override;
    int tx_number() override;
    std::string to_string() override;
    void undo(transaction* pTx) override;
    static int write_to_log(log_manager* pLM,
                            const int &pTxNum);
  private:
    int mTxNum;
  };

  class rollback_record: public log_record {
  public:
    rollback_record(page* pPage);
    int op() override;
    int tx_number() override;
    std::string to_string() override;
    void undo(transaction* pTx) override;
    static int write_to_log(log_manager* pLM,
                            const int &pTxNum);
  private:
    int mTxNum;
  };

  
  class set_int_record: public log_record {
  public:
    set_int_record(page* pPgae);
    int op() override;
    int tx_number() override;
    std::string to_string() override;
    void undo(transaction* pTx) override;
    static int write_to_log(log_manager* pLM,
                            const int &pTxNum,
                            const block_id & pBlockId,
                            const int &pOffset,
                            const int &pVal);

  private:
    int mTxNum, mOffset;
    int mVal;
    block_id mBlockId;
  };

  class set_string_record: public log_record {
  public:
    set_string_record(page* pPage);
    int op() override;
    int tx_number() override;
    std::string to_string() override;
    void undo(transaction* pTx) override;
    static int write_to_log(log_manager* pLM,
                            const int &pTxNum,
                            const block_id & pBlockId,
                            const int &pOffset,
                            const std::string &pVal);
    
  private:
    int mTxNum, mOffset;
    std::string mVal;
    block_id mBlockId;
  };
  
}

