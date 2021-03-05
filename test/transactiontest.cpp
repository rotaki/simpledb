#include <iostream>

#include "server/smartdb.hpp"
#include "buffer/buffermanager.hpp"
#include "file/blockid.hpp"
#include "file/filemanager.hpp"
#include "file/page.hpp"
#include "log/logmanager.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  TEST(tx, transactiontest) {
    smartdb db("txtest", 400, 8);
    file_manager* fM = db.file_mgr();
    log_manager* lM = db.log_mgr();
    buffer_manager* bM = db.buffer_mgr();

    auto tx1 = std::make_unique<transaction>(fM, lM, bM);
    block_id blk("testfile", 1);
    tx1->pin(blk);

    // The block initially contains unkown bytes
    // so don't log those values here
    tx1->set_int(blk, 80, 1, false);
    tx1->set_string(blk, 40, "one", false);
    tx1->commit();

    auto tx2 = std::make_unique<transaction>(fM, lM, bM);
    tx2->pin(blk);
    int iVal = tx2->get_int(blk, 80);
    std::string sVal = tx2->get_string(blk, 40);
    std::cout << "inital value at location 80 = " << iVal << std::endl;
    std::cout << "inital value at location 40 =" << sVal << std::endl;
    int newIVal = iVal + 1;
    std::string newSVal = sVal + "!";
    tx2->set_int(blk, 80, newIVal, true);
    tx2->set_string(blk, 40, newSVal, true);
    tx2->commit();

    auto tx3 = std::make_unique<transaction>(fM, lM, bM);
    tx3->pin(blk);
    std::cout << "new value at location 80 = " << tx3->get_int(blk, 80) << std::endl;
    std::cout << "new value at location 40 = " << tx3->get_string(blk, 40) << std::endl;
    tx3->set_int(blk, 80, 9999, true);
    std::cout << "pre-rollback value at location 80 = " << tx3->get_int(blk, 80) << std::endl;
    tx3->rollback();

    auto tx4 = std::make_unique<transaction>(fM, lM, bM);
    tx4->pin(blk);
    std::cout << "post-rollback at location 80 = " << tx4->get_int(blk, 80) << std::endl;
    tx4->commit();
  }
}
