#include <random>

#include "tablemanager.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  TEST(tablemanager, basic) {
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

    table_manager tM(true, tx);
    tM.create_table("MyTable", sch, tx);
    std::shared_ptr<layout> lt = tM.get_layout("MyTable", tx);
    int size = lt->slot_size();
    
    std::shared_ptr<schema> sch2 = lt->get_schema();
    std::cout << "MyTable has slot size " << size << std::endl;
    std::cout << "Its fields are: " << std::endl;
    for (const std::string &pFldName: sch2->fields()) {
      std::string type;
      if (sch2->type(pFldName) == schema::integer) {
        type = "int";
      } else {
        int strlen = sch2->length(pFldName);
        type = "varchar(" + std::to_string(strlen) + ")";
      }
      std::cout << pFldName << ": " << type << std::endl;
    }
    tx->commit();
  }
}
