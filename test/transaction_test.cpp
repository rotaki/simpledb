#include <filesystem>

#include "transaction.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  TEST(transaction, basic) {
    int blockSize = 400;
    const auto oldCurrentPath = fs::current_path();
    const auto dirSandBox = oldCurrentPath / "sandbox";
    fs::remove_all(dirSandBox);
    fs::create_directory(dirSandBox);
    std::string logFileName = "logfile.dat";
    std::string testFileName = "testfile.dat";

    std::shared_ptr<file_manager> fM(new file_manager(dirSandBox, blockSize));
    std::shared_ptr<log_manager> lM(new log_manager(fM, logFileName));
    int bufferNums = 8;
    std::shared_ptr<buffer_manager> bM(new buffer_manager(fM, lM, bufferNums));
    
    auto tx1 = transaction::create(fM, lM, bM);
    std::shared_ptr<block_id> blockId(new block_id(testFileName, 1));
    tx1->pin(blockId);
    tx1->set_int(blockId, 80, 1, false);
    tx1->set_string(blockId, 40, "one", false);
    tx1->commit();

    auto tx2 = transaction::create(fM, lM, bM);
    tx2->pin(blockId);
    int iVal = tx2->get_int(blockId, 80);
    std::string sVal = tx2->get_string(blockId, 40);
    
    EXPECT_EQ(1, iVal);
    EXPECT_EQ("one", sVal);

    int newIVal = iVal + 1;
    std::string newSVal = sVal + "!";
    tx2->set_int(blockId, 80, newIVal, true);
    tx2->set_string(blockId, 40, newSVal, true);
    tx2->commit();

    auto tx3 = transaction::create(fM, lM, bM);
    tx3->pin(blockId);
    EXPECT_EQ(newIVal, tx3->get_int(blockId, 80));
    EXPECT_EQ(newSVal, tx3->get_string(blockId, 40));

    tx3->set_int(blockId, 80, 9999, true);
    EXPECT_EQ(9999, tx3->get_int(blockId, 80));

    tx3->rollback();

    auto tx4 = transaction::create(fM, lM, bM);
    tx4->pin(blockId);
    EXPECT_EQ(newIVal, tx4->get_int(blockId, 80));

    tx4->commit();
    fs::remove_all(dirSandBox);
  }
}
