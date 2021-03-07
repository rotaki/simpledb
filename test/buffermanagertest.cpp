#include <iostream>

#include "buffer/buffermanager.hpp"
#include "file/blockid.hpp"
#include "file/filemanager.hpp"
#include "file/page.hpp"
#include "server/simpledb.hpp"
#include "gtest/gtest.h"

namespace simpledb {
TEST(buffer, buffermanager) {
  simpledb db("buffermgrtest", 400, 3); // three buffers
  buffer_manager *bM = db.buffer_mgr();

  std::vector<buffer *> buff(6);
  std::string testFile = "testfile";
  buff[0] = bM->pin(block_id(testFile, 0));
  buff[1] = bM->pin(block_id(testFile, 1));
  buff[2] = bM->pin(block_id(testFile, 2));

  bM->unpin(buff[1]);
  buff[1] = nullptr;

  buff[3] = bM->pin(block_id(testFile, 0));
  buff[4] = bM->pin(block_id(testFile, 1));
  std::cout << "Available buffers " << bM->available() << std::endl;
  try {
    std::cout << "Attempting to pin block 3" << std::endl;
    buff[5] = bM->pin(block_id(testFile, 3));
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  bM->unpin(buff[2]);
  buff[2] = nullptr;
  buff[5] = bM->pin(block_id(testFile, 3));
  std::cout << "Final Buffer Allocation" << std::endl;

  for (int i = 0; i < buff.size(); i++) {
    buffer *b = buff[i];
    if (b != nullptr) {
      std::cout << "buff[" << i << "] pinned to block" << b->block().to_string()
                << std::endl;
    }
  }
}
} // namespace simpledb
