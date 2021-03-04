#pragma once

#include <memory>
#include <vector>
#include <string>

#include "file/blockid.hpp"
#include "file/page.hpp"
#include "log/logmanager.hpp"

namespace smartdb {

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
    virtual void undo(std::shared_ptr<transaction> pTx) = 0;
    static std::shared_ptr<log_record> create_log_record(std::vector<char> &pByteVec);
  };

  class checkpoint_record: public log_record {
  public:
    checkpoint_record();
    int op() override;
    int tx_number() override;
    std::string to_string();
    void undo(std::shared_ptr<transaction> pTx) override;
    static int write_to_log(std::shared_ptr<log_manager> pLM);
  };

  class start_record: public log_record {
  public:
    start_record(std::shared_ptr<page> pPage);
    int op() override;
    int tx_number() override;
    std::string to_string();
    void undo(std::shared_ptr<transaction> pTx) override;
    static int write_to_log(std::shared_ptr<log_manager> pLM,
                            const int &pTxNum);
  private:
    int mTxNum;
  };

  class commit_record: public log_record {
  public:
    commit_record(std::shared_ptr<page> pPage);
    int op() override;
    int tx_number() override;
    std::string to_string();
    void undo(std::shared_ptr<transaction> pTx) override;
    static int write_to_log(std::shared_ptr<log_manager> pLM,
                            const int &pTxNum);
  private:
    int mTxNum;
  };

  class rollback_record: public log_record {
  public:
    rollback_record(std::shared_ptr<page> pPage);
    int op() override;
    int tx_number() override;
    std::string to_string();
    void undo(std::shared_ptr<transaction> pTx) override;
    static int write_to_log(std::shared_ptr<log_manager> pLM,
                            const int &pTxNum);
  private:
    int mTxNum;
  };

  
  class set_int_record: public log_record {
  public:
    set_int_record(std::shared_ptr<page> pPgae);
    int op() override;
    int tx_number() override;
    std::string to_string();
    void undo(std::shared_ptr<transaction> pTx) override;
    static int write_to_log(std::shared_ptr<log_manager> pLM,
                            const int &pTxNum,
                            std::shared_ptr<block_id> pBlockId,
                            const int &pOffset,
                            const int &pVal);

  private:
    int mTxNum, mOffset;
    int mVal;
    std::shared_ptr<block_id> mBlockId;
  };

  class set_string_record: public log_record {
  public:
    set_string_record(std::shared_ptr<page> pPage);
    int op() override;
    int tx_number() override;
    std::string to_string();
    void undo(std::shared_ptr<transaction> pTx) override;
    static int write_to_log(std::shared_ptr<log_manager> pLM,
                            const int &pTxNum,
                            std::shared_ptr<block_id> pBlockId,
                            const int &pOffset,
                            const std::string &pVal);
    
  private:
    int mTxNum, mOffset;
    std::string mVal;
    std::shared_ptr<block_id> mBlockId;
  };
  
}

