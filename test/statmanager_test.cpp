#include <random>

#include "tablemanager.hpp"
#include "tablescan.hpp"
#include "statmanager.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  TEST(statmanager, basic) {
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
    sch->add_string_field("name", 10);
    sch->add_int_field("majorid");

    std::shared_ptr<table_manager> tM(new table_manager(true, tx));
    tM->create_table("student", sch, tx);
    std::shared_ptr<stat_manager> sM(new stat_manager(tM, tx));
    std::shared_ptr<layout> lt = tM->get_layout("student", tx);
    std::shared_ptr<stat_info> sI = sM->get_stat_info("student", lt, tx);
    std::cout << sI->blocks_accessed() << " " << sI->records_output() << " " << sI->distinct_values("majorid") << std::endl;
    tx->commit();

    fs::remove_all(dirSandBox);
  }
}
