#include <random>

#include "tablescan.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  TEST(tablescan, basic) {
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

    tablescan ts(tx, "T", lt);
    std::cout <<"filling the page with 50 random records." << std::endl;
    ts.before_first();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> d(0, 1);
    
    for (int i = 0; i< 50; i++) {
      ts.insert();
      int n = round(d(gen) * 50);
      ts.set_int("A", n);
      ts.set_string("B", "rec"+std::to_string(n));
      std::cout << "inserting into slot " << ts.get_rid().to_string() <<  ": {" << n << ", rec" << n <<  "}" << std::endl;
    }

    std::cout << "deleted these records with A-values < 25." << std::endl;
    int count = 0;
    ts.before_first();
    while (ts.next()) {
      int a = ts.get_int("A");
      std::string b = ts.get_string("B");
      if (a < 25) {
        count++;
        std::cout << "slot " << ts.get_rid().to_string() << ": {" << a << ", " << b <<  "}" << std::endl;
        ts.remove();
      }
    }
    std::cout << count << " values under 25 were deleted" << std::endl;
    std::cout << " here are the remaining records" << std::endl;
    ts.before_first();
    while(ts.next()) {
      int a = ts.get_int("A");
      std::string b = ts.get_string("B");
      std::cout << "slot " << ts.get_rid().to_string() << ": {" << a << ", " << b << "}" << std::endl;
    }
    ts.close();
    tx->commit();
  }
}
