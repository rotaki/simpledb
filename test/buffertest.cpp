#include <iostream>

#include "buffer/buffermanager.hpp"
#include "server/smartdb.hpp"
#include "file/blockid.hpp"
#include "file/filemanager.hpp"
#include "file/page.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  TEST(buffer, buffer) {
    smartdb db("buffertest", 400, 3); // three buffers
    buffer_manager* bM = db.buffer_mgr();

    std::string testFile = "testfile";
    buffer* buff1 = bM->pin(block_id(testFile, 1));
    page* p = buff1->contents();
    int n = p->get_int(80);
    p->set_int(80, n+1);
    buff1->set_modified(1, 0);
    std::cout << "The new value is " << (n+1) << std::endl;
    bM->unpin(buff1);

    buffer* buff2 = bM->pin(block_id(testFile, 2));
    buffer* buff3 = bM->pin(block_id(testFile, 3));
    buffer* buff4 = bM->pin(block_id(testFile, 4));

    bM->unpin(buff2);
    buff2 = bM->pin(block_id(testFile, 1));
    page* p2 = buff2->contents();
    p2->set_int(80, 9999);
    buff2->set_modified(1, 0);
  }
}
