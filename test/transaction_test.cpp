#include <filesystem>

#include "transaction.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  TEST(transaction, hoge) {
    int blockSize = 400;
    const auto oldCurrentPath = fs::current_path();
    const auto dirSandBox = oldCurrentPath / "sandbox";
    fs::remove_all(dirSandBox);
    fs::create_directory(dirSandBox);
    std::string logFileName = "logfile.dat";
    std::string testFileName = "testfile.dat";
    // auto fM = std::make_shared<file_manager>(dirSandBox, blockSize);
    // auto lM = std::make_shared<buffer_manager>(fM, logFileName);
    std::shared_ptr<file_manager> fM(new file_manager(dirSandBox, blockSize));
    std::shared_ptr<log_manager> lM(new log_manager(fM, logFileName));
    int bufferNums = 8;
    // auto bM = std::make_shared<buffer_manager>(fM, lM, 8);
    std::shared_ptr<buffer_manager> bM(new buffer_manager(fM, lM, 8));
    auto tx1 = transaction::create(fM, lM, bM);
    //tx1->start();
    std::shared_ptr<block_id> blockId(new block_id(testFileName, 1));
    tx1->pin(blockId);
    tx1->set_int(blockId, 80, 1, false);
    tx1->set_string(blockId, 40, "one", false);
    tx1->commit();
  }
}
