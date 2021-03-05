#include <iostream>

#include "buffer/buffermanager.hpp"
#include "server/smartdb.hpp"
#include "file/blockid.hpp"
#include "file/filemanager.hpp"
#include "file/page.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  TEST(buffer, bufferfile) {
    smartdb db("bufferfiletest", 400, 3); // three buffers
    buffer_manager* bM = db.buffer_mgr();

    block_id blk("testfile", 2);
    int pos1 = 88;

    buffer* b1 = bM->pin(blk);
    page* p1 = b1->contents();
    std::string sample =  "abcdefghijklm";
    p1->set_string(pos1, sample);
    int size = page::max_length(sample.size());
    int pos2 = pos1 + size;
    p1->set_int(pos2, 345);
    b1->set_modified(1, 0);
    bM->unpin(b1);

    buffer* b2 = bM->pin(blk);
    page* p2 = b2->contents();
    std::cout << "offset " << pos2 << " contains " << p2->get_int(pos2) << std::endl;
    std::cout << "offset " << pos1 << " contains " << p2->get_string(pos1) << std::endl;
    bM->unpin(b1);
  }
}
