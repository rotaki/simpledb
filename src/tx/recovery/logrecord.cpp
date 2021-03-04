#include "tx/recovery/logrecord.hpp"
#include "tx/transaction.hpp"

namespace smartdb {
  log_record::~log_record() {}
  
  std::shared_ptr<log_record> log_record::create_log_record(std::vector<char> &pByteVec) {
    std::shared_ptr<page> p(new page(pByteVec));
    operation oper = static_cast<operation>(p->get_int(0));
    switch (oper) {
    case checkpoint:
      return std::shared_ptr<log_record>(new checkpoint_record);
    case start:
      return std::shared_ptr<log_record>(new start_record(p));
    case commit:
      return std::shared_ptr<log_record>(new commit_record(p));
    case rollback:
      return std::shared_ptr<log_record>(new rollback_record(p));
    case setint:
      return std::shared_ptr<log_record>(new set_int_record(p));
    case setstring:
      return std::shared_ptr<log_record>(new set_string_record(p));
    default:
      return std::shared_ptr<log_record>(nullptr);
    }
  }

  checkpoint_record::checkpoint_record() {
    // do nothing
  }

  int checkpoint_record::op() {
    return checkpoint;
  }

  int checkpoint_record::tx_number() {
    return -1;                  // dummy
  }

  std::string checkpoint_record::to_string() {
    return "<CHECKPOINT>";
  }

  void checkpoint_record::undo(std::shared_ptr<transaction> pTx) {
    // do nothing;
  }

  int checkpoint_record::write_to_log(std::shared_ptr<log_manager> pLM) {
    std::vector<char> byteVec(sizeof(int), 0);
    std::shared_ptr<page> p(new page(byteVec));
    p->set_int(0, checkpoint);
    std::vector<char> rec = p->contents();
    return pLM->append(rec);
  }
  
  start_record::start_record(std::shared_ptr<page> pPage) {
    int tPos = sizeof(int);
    mTxNum = pPage->get_int(tPos);
  }

  int start_record::op() {
    return start;
  }

  int start_record::tx_number() {
    return mTxNum;
  }

  std::string start_record::to_string() {
    return "<START, " + std::to_string(mTxNum) + ">";
  }

  void start_record::undo(std::shared_ptr<transaction> pTx)  {
    // do nothing
  }

  int start_record::write_to_log(std::shared_ptr<log_manager> pLM,
                                 const int &pTxNum) {
    int tPos = sizeof(int);
    int recLen = tPos + sizeof(int);
    std::vector<char> byteVec(recLen, 0);
    std::shared_ptr<page> p(new page(byteVec));
    p->set_int(0, start);
    p->set_int(tPos, pTxNum);
    std::vector<char> rec = p->contents();
    return pLM->append(rec);
  }

  commit_record::commit_record(std::shared_ptr<page> pPage) {
    int tPos = sizeof(int);
    mTxNum = pPage->get_int(tPos);
  }

  int commit_record::op() {
    return commit;
  }

  int commit_record::tx_number() {
    return mTxNum;
  }

  std::string commit_record::to_string() {
    return "<COMMIT, " + std::to_string(mTxNum) + ">";
  }

  void commit_record::undo(std::shared_ptr<transaction> pTx) {
    // do nothing
  }

  int commit_record::write_to_log(std::shared_ptr<log_manager> pLM,
                                  const int &pTxNum) {
    int tPos = sizeof(int);
    int recLen = tPos + sizeof(int);
    std::vector<char> byteVec(recLen, 0);
    std::shared_ptr<page> p(new page(byteVec));
    p->set_int(0, start);
    p->set_int(tPos, pTxNum);
    std::vector<char> rec = p->contents();
    return pLM->append(rec);
  }

  rollback_record::rollback_record(std::shared_ptr<page> pPage) {
    int tPos = sizeof(int);
    mTxNum = pPage->get_int(tPos);
  }

  int rollback_record::op() {
    return rollback;
  }

  int rollback_record::tx_number() {
    return mTxNum;
  }

  std::string rollback_record::to_string() {
    return "<ROLLBACK, " + std::to_string(mTxNum) + ">";
  }

  void rollback_record::undo(std::shared_ptr<transaction> pTx) {
    // do nothing
  }
  
  int rollback_record::write_to_log(std::shared_ptr<log_manager> pLM,
                                    const int &pTxNum) {
    int tPos = sizeof(int);
    int recLen = tPos + sizeof(int);
    std::vector<char> byteVec(recLen, 0);
    std::shared_ptr<page> p(new page(byteVec));
    p->set_int(0, start);
    p->set_int(tPos, pTxNum);
    std::vector<char> rec = p->contents();
    return pLM->append(rec);
  }

  set_int_record::set_int_record(std::shared_ptr<page> pPage) {
    int tPos = sizeof(int);
    mTxNum = pPage->get_int(tPos);
    int fPos = tPos + sizeof(int);
    std::string fileName = pPage->get_string(fPos);
    int bPos = fPos + page::max_length(fileName.size());
    int blkNum = pPage->get_int(bPos);
    mBlockId = std::shared_ptr<block_id>(new block_id(fileName, blkNum));
    int oPos = bPos + sizeof(int);
    mOffset = pPage->get_int(oPos);
    int vPos = oPos + sizeof(int);
    mVal = pPage->get_int(vPos);
  }

  int set_int_record::op() {
    return setint;
  }

  int set_int_record::tx_number() {
    return mTxNum;
  }

  std::string set_int_record::to_string() {
    return "<SETINT, " + std::to_string(mTxNum) + ", " + mBlockId->to_string() + ", " + std::to_string(mOffset) + ", " + std::to_string(mVal) + ">";
  }

  void set_int_record::undo(std::shared_ptr<transaction> pTx) {
    pTx->pin(mBlockId);
    pTx->set_int(mBlockId, mOffset, mVal, false);
    pTx->unpin(mBlockId);
  }

  int set_int_record::write_to_log(std::shared_ptr<log_manager> pLM,
                                   const int &pTxNum,
                                   std::shared_ptr<block_id> pBlockId,
                                   const int &pOffset,
                                   const int &pVal) {
    int tPos = sizeof(int);
    int fPos = tPos + sizeof(int);
    int bPos = fPos + page::max_length(pBlockId->file_name().size());
    int oPos = bPos + sizeof(int);
    int vPos = oPos + sizeof(int);
    int recLen = vPos + sizeof(int);
    std::vector<char> byteVec(recLen, 0);
    std::shared_ptr<page> p(new page(byteVec));
    p->set_int(0, setint);
    p->set_int(tPos, pTxNum);
    p->set_string(fPos, pBlockId->file_name());
    p->set_int(bPos, pBlockId->number());
    p->set_int(oPos, pOffset);
    p->set_int(vPos, pVal);
    std::vector<char> rec = p->contents();
    return pLM->append(rec);
  }

  set_string_record::set_string_record(std::shared_ptr<page> pPage) {
    int tPos = sizeof(int);
    mTxNum = pPage->get_int(tPos);
    int fPos = tPos + sizeof(int);
    std::string fileName = pPage->get_string(fPos);
    int bPos = fPos + page::max_length(fileName.size());
    int blkNum = pPage->get_int(bPos);
    mBlockId = std::shared_ptr<block_id>(new block_id(fileName, blkNum));
    int oPos = bPos + sizeof(int);
    mOffset = pPage->get_int(oPos);
    int vPos = oPos + sizeof(int);
    mVal = pPage->get_string(vPos);
  }

  int set_string_record::op() {
    return setstring;
  }

  int set_string_record::tx_number() {
    return mTxNum;
  }

  std::string set_string_record::to_string() {
    return "<SETSTRING, " + std::to_string(mTxNum) + ", " + mBlockId->to_string() + ", " + std::to_string(mOffset) + ", " + mVal + ">";
  }

  void set_string_record::undo(std::shared_ptr<transaction> pTx) {
    pTx->pin(mBlockId);
    pTx->set_string(mBlockId, mOffset, mVal, false);
    pTx->unpin(mBlockId);
  }

  int set_string_record::write_to_log(std::shared_ptr<log_manager> pLM,
                                      const int &pTxNum,
                                      std::shared_ptr<block_id> pBlockId,
                                      const int &pOffset,
                                      const std::string &pVal) {
    int tPos = sizeof(int);
    int fPos = tPos + sizeof(int);
    int bPos = fPos + page::max_length(pBlockId->file_name().size());
    int oPos = bPos + sizeof(int);
    int vPos = oPos + sizeof(int);
    int recLen = vPos + page::max_length(pVal.size());
    std::vector<char> byteVec(recLen, 0);
    std::shared_ptr<page> p(new page(byteVec));
    p->set_int(0, setstring);
    p->set_int(tPos, pTxNum);
    p->set_string(fPos, pBlockId->file_name());
    p->set_int(bPos, pBlockId->number());
    p->set_int(oPos, pOffset);
    p->set_string(vPos, pVal);
    std::vector<char> rec = p->contents();
    return pLM->append(rec);
  }  
}
