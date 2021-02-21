#include <filesystem>
#include <chrono>
#include <thread>
#include <iostream>

#include "transaction.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  void run_thread1(std::shared_ptr<transaction> tx,
                   std::shared_ptr<block_id> pBlockId1,
                   std::shared_ptr<block_id> pBlockId2) {
    using namespace std::chrono_literals;
    tx->pin(pBlockId1);
    tx->pin(pBlockId2);
    std::cout << "Tx 1: request slock 1" << std::endl;
    tx->get_int(pBlockId1, 0);
    std::cout << "Tx 1: receive slock 1" << std::endl;
    std::this_thread::sleep_for(1000ms);
    std::cout << "Tx 1: request slock 2" << std::endl;
    tx->get_int(pBlockId2, 0);
    std::cout << "Tx 1: receive slock 2" << std::endl;
    tx->commit();
  }

  void run_thread2(std::shared_ptr<transaction> tx,
                   std::shared_ptr<block_id> pBlockId1,
                   std::shared_ptr<block_id> pBlockId2) {
    using namespace std::chrono_literals;
    tx->pin(pBlockId1);
    tx->pin(pBlockId2);
    std::cout << "Tx 2: request xlock 2" << std::endl;
    tx->set_int(pBlockId2, 0, 0, false);
    std::cout << "Tx 2: receive xlock 2" << std::endl;
    std::this_thread::sleep_for(1000ms);
    std::cout << "Tx 2: request slock 1" << std::endl;
    tx->get_int(pBlockId1, 0);
    std::cout << "Tx 2: receive slock 1" << std::endl;
    tx->commit();
  }

  void run_thread3(std::shared_ptr<transaction> tx,
                   std::shared_ptr<block_id> pBlockId1,
                   std::shared_ptr<block_id> pBlockId2
                   ) {
    using namespace std::chrono_literals;
    tx->pin(pBlockId1);
    tx->pin(pBlockId2);
    std::cout << "Tx 3: request xlock 1" << std::endl;
    tx->set_int(pBlockId1, 0, 0, false);
    std::cout << "Tx 3: receive xlock 1" << std::endl;
    std::this_thread::sleep_for(1000ms);
    std::cout << "Tx 3: request slock 2" << std::endl;
    tx->get_int(pBlockId1, 0);
    std::cout << "Tx 3: receive slock 2" << std::endl;
    tx->commit();
  }
  
  TEST(concurrencymanager, basic) {
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
    auto tx2 = transaction::create(fM, lM, bM);
    auto tx3 = transaction::create(fM, lM, bM);

    std::shared_ptr<block_id> blockId1(new block_id(testFileName, 1));
    std::shared_ptr<block_id> blockId2(new block_id(testFileName, 2));
    
    std::thread thread1(run_thread1, tx1, blockId1, blockId2);
    std::thread thread2(run_thread2, tx2, blockId1, blockId2);
    std::thread thread3(run_thread3, tx3, blockId1, blockId2);

    thread1.join();
    thread2.join();
    thread3.join();
    
    fs::remove_all(dirSandBox);
  }
}
