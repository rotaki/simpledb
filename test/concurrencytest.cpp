#include <iostream>
#include <thread>

#include "server/smartdb.hpp"
#include "file/blockid.hpp"
#include "file/filemanager.hpp"
#include "file/page.hpp"
#include "log/logmanager.hpp"
#include "buffer/buffermanager.hpp"
#include "tx/transaction.hpp"
#include "gtest/gtest.h"

using namespace std::chrono_literals;

namespace smartdb {
  void run_A(file_manager* fM, log_manager* lM, buffer_manager* bM) {
    try {
      auto txA = std::make_unique<transaction>(fM, lM, bM);
      std::string testFile = "testfile";
      block_id blk1(testFile, 1);
      block_id blk2(testFile, 2);
      txA->pin(blk1);
      txA->pin(blk2);
      std::cout << "Tx A: request slock 1" << std::endl;;
      txA->get_int(blk1, 0);
      std::cout << "Tx A: receive slock 1" << std::endl;
      std::this_thread::sleep_for(1000ms);
      std::cout << "Tx A: request slock 2" << std::endl;
      txA->get_int(blk2, 0);
      std::cout << "Tx A: receive slock 2" << std::endl;
      txA->commit();
      std::cout << "Tx A: commit" << std::endl;
    } catch (std::exception & e){
      std::cout << e.what() << std::endl;
    }
  }

  void run_B(file_manager* fM, log_manager* lM, buffer_manager* bM) {
    try {
      auto txB = std::make_unique<transaction>(fM, lM, bM);
      std::string testFile = "testfile";
      block_id blk1(testFile, 1);
      block_id blk2(testFile, 2);
      txB->pin(blk1);
      txB->pin(blk2);
      std::cout << "Tx B: request xlock 2" << std::endl;;
      txB->set_int(blk2, 0, 0, false);
      std::cout << "Tx B: receive xlock 2" << std::endl;
      std::this_thread::sleep_for(1000ms);
      std::cout << "Tx B: request slock 1" << std::endl;
      txB->get_int(blk1, 0);
      std::cout << "Tx B: receive slock 1" << std::endl;
      txB->commit();
      std::cout << "Tx B: commit" << std::endl;
    } catch (std::exception & e){
      std::cout << e.what() << std::endl;
    }
  }
  
  void run_C(file_manager* fM, log_manager* lM, buffer_manager* bM) {
    try {
      auto txC = std::make_unique<transaction>(fM, lM, bM);
      std::string testFile = "testfile";
      block_id blk1(testFile, 1);
      block_id blk2(testFile, 2);
      txC->pin(blk1);
      txC->pin(blk2);
      std::this_thread::sleep_for(500ms);
      std::cout << "Tx C: request xlock 1" << std::endl;;
      txC->set_int(blk1, 0, 0, false);
      std::cout << "Tx C: receive xlock 1" << std::endl;
      std::this_thread::sleep_for(1000ms);
      std::cout << "Tx C: request slock 2" << std::endl;
      txC->get_int(blk2, 0);
      std::cout << "Tx C: receive slock 2" << std::endl;
      txC->commit();
      std::cout << "Tx C: commit" << std::endl;
    } catch (std::exception & e){
      std::cout << e.what() << std::endl;
    }
  }

  
  TEST(tx, concurrencytest) {
    smartdb db("concurrencytest", 400, 8);
    file_manager* fM = db.file_mgr();
    log_manager* lM = db.log_mgr();
    buffer_manager* bM = db.buffer_mgr();
    
    std::thread A(run_A, fM, lM, bM);
    std::thread B(run_B, fM, lM, bM);
    std::thread C(run_C, fM, lM, bM);

    A.join();
    B.join();
    C.join();
  }
}

