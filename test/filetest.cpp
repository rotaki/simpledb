#include <iostream>

#include "file/blockid.hpp"
#include "file/filemanager.hpp"
#include "file/page.hpp"
#include "server/simpledb.hpp"
#include "gtest/gtest.h"

namespace simpledb {
TEST(file, file_test) {
  simpledb db("filetest", 400, 8);
  file_manager &fM = db.file_mgr();
  block_id blk("testfile", 2);
  int pos1 = 88;

  auto p1 = std::make_unique<page>(fM.block_size());
  std::string sample = "abcdefghijklm";
  p1->set_string(pos1, sample);
  int size = page::max_length(sample.size());
  int pos2 = pos1 + size;
  p1->set_int(pos2, 345);
  fM.write(blk, *p1);

  auto p2 = std::make_unique<page>(fM.block_size());
  fM.read(blk, *p2);
  std::cout << "offset " << pos2 << " contains " << p2->get_int(pos2)
            << std::endl;
  std::cout << "offset " << pos1 << " contains " << p2->get_string(pos1)
            << std::endl;
}
} // namespace simpledb
