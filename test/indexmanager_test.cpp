#include <random>

#include "tablemanager.hpp"
#include "tablescan.hpp"
#include "indexmanager.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  TEST(indexmanager, basic) {
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
    sch->add_int_field("sid");
    sch->add_string_field("sname", 10);

    std::shared_ptr<table_manager> tM(new table_manager(true, tx));
    tM->create_table("student", sch, tx);
    std::shared_ptr<stat_manager> sM(new stat_manager(tM, tx));
    std::shared_ptr<index_manager> iM(new index_manager(true, tM, sM, tx));
    iM->create_index("sidIdx", "student", "sid", tx);
    iM->create_index("snameIdx", "student", "sname", tx);

    std::map<std::string, std::shared_ptr<index_info>> indexes = iM->get_index_info("student", tx);
    for (const auto & [fldName, iI]: indexes) {
      std::cout << fldName << " " << iI->blocks_accessed() << std::endl;
    }
    tx->commit();
    fs::remove_all(dirSandBox);
  }
}
