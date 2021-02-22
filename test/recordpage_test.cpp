#include <random>

#include "recordpage.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  TEST(recordpage, basic) {
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
    auto tx = transaction::create(fM, lM, bM);

    std::shared_ptr<schema> sch(new schema);
    sch->add_int_field("A");
    sch->add_string_field("B", 9);
    std::shared_ptr<layout> lt(new layout(sch));
    
    for (const std::string &fldName: lt->get_schema()->fields()) {
      int offset = lt->offset(fldName);
      std::cout << fldName + " has offset " << offset << std::endl;
    }

    std::shared_ptr<block_id> blockId = tx->append(testFileName);
    tx->pin(blockId);
    record_page rP(tx, blockId, lt);
    rP.format();

    std::cout <<"filling the page with random records." << std::endl;

    int slot = rP.insert_after(-1);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> d(0, 1);
    
    while (slot >= 0) {
      int n = round(d(gen) * 50);
      rP.set_int(slot, "A", n);
      rP.set_string(slot, "B", "rec"+std::to_string(n));
      std::cout << "inserting into slot " << slot <<  ": {" << n << ", rec" << n <<  "}" << std::endl;
      slot = rP.insert_after(slot);
    }

    std::cout << "deleted these records with A-values < 25." << std::endl;
    int count = 0;
    slot = rP.next_after(-1);
    while (slot >= 0) {
      int a = rP.get_int(slot, "A");
      std::string b = rP.get_string(slot, "B");
      if (a < 25) {
        count++;
        std::cout << "slot " << slot << ": {" << a << ", " << b <<  "}" << std::endl;
        rP.remove(slot);
      }
      slot = rP.next_after(slot);
    }

    std::cout << count << " values under 25 were deleted" << std::endl;
    std::cout << " here are the remaining records" << std::endl;
    slot = rP.next_after(-1);
    while (slot >= 0) {
      int a = rP.get_int(slot, "A");
      std::string b = rP.get_string(slot, "B");
      std::cout << "slot " << slot << ": {" << a << ", " << b << "}" << std::endl;
      slot = rP.next_after(slot);
    }

    tx->unpin(blockId);
    tx->commit();
  }
}
