#include "file/blockid.hpp"
#include "file/filemanager.hpp"
#include "file/page.hpp"
#include "server/simpledb.hpp"
#include "gtest/gtest.h"

namespace simpledb {
class tx_recovery : public ::testing::Test {
public:
  tx_recovery()
      : mDb("recoverytest", 400, 8), mTestFile("testfile"), mFM(mDb.file_mgr()),
        mBM(mDb.buffer_mgr()), mBlk0(mTestFile, 0), mBlk1(mTestFile, 1) {
    if (mFM.length(mTestFile) == 0) {
      initialize();
      modify();
    } else {
      recover();
    }
  }

  void initialize() {
    auto tx1 = mDb.new_tx();
    auto tx2 = mDb.new_tx();
    tx1->pin(mBlk0);
    tx2->pin(mBlk1);
    int pos = 0;
    for (int i = 0; i < 6; i++) {
      tx1->set_int(mBlk0, pos, pos, false);
      tx2->set_int(mBlk1, pos, pos, false);
      pos += sizeof(int);
    }
    tx1->set_string(mBlk0, 30, "abc", false);
    tx2->set_string(mBlk1, 30, "def", false);
    tx1->commit();
    tx2->commit();
    print_values("After Initialization");
  }

  void modify() {
    auto tx3 = mDb.new_tx();
    auto tx4 = mDb.new_tx();
    tx3->pin(mBlk0);
    tx4->pin(mBlk1);
    int pos = 0;
    for (int i = 0; i < 6; i++) {
      tx3->set_int(mBlk0, pos, pos + 100, true);
      tx4->set_int(mBlk1, pos, pos + 100, true);
      pos += sizeof(int);
    }
    tx3->set_string(mBlk0, 30, "uvw", true);
    tx4->set_string(mBlk1, 30, "xyz", true);
    mBM.flush_all(tx3->num());
    mBM.flush_all(tx4->num());
    print_values("After modification");

    tx3->rollback();
    print_values("After rollback");
  }

  void recover() {
    auto tx = mDb.new_tx();
    tx->recover();
    print_values("After recovery");
  }

  void print_values(const std::string &pMsg) {
    std::cout << pMsg << std::endl;
    page p0(mFM.block_size());
    page p1(mFM.block_size());
    mFM.read(mBlk0, p0);
    mFM.read(mBlk1, p1);
    int pos = 0;
    for (int i = 0; i < 6; i++) {
      std::cout << p0.get_int(pos) << " ";
      std::cout << p1.get_int(pos) << " ";
      pos += sizeof(int);
    }
    std::cout << p0.get_string(30) << " ";
    std::cout << p1.get_string(30) << " ";
    std::cout << std::endl;
  }

private:
  simpledb mDb;
  std::string mTestFile;
  file_manager &mFM;
  buffer_manager &mBM;
  block_id mBlk0, mBlk1;
};

TEST_F(tx_recovery, recovery_test) {}
} // namespace simpledb
