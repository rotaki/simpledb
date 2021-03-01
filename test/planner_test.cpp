#include <random>
#include <iostream>
#include <sstream>
#include <filesystem>

#include "smartdb.hpp"
#include "transaction.hpp"
#include "planner.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  TEST(planner, basic) {
    fs::remove_all(fs::current_path() / "studentdb");
    smartdb db("studentdb");
    std::shared_ptr<planner> plnr = db.new_planner();
    std::shared_ptr<transaction> tx = db.new_tx();
    std::shared_ptr<schema> sch(new schema);
    sch->add_string_field("sname", 10);
    sch->add_int_field("gradyear");
    sch->add_int_field("MajorId");

    // auto mM = db.new_mm();
    // mM->create_table("student", sch, tx);

    // auto lt = mM->get_layout("student", tx);
    
    // std::shared_ptr<table_scan> tS(new table_scan(tx, "student", lt));
    // for (int i = 0; i < 50; i++) {
    //   tS->insert();
    //   tS->set_int("gradyear", i);
    //   tS->set_string("sname", "student"+std::to_string(i));
    //   tS->set_int("majorid", i);
      
    // }

    std::string cmd = "create table student (gradyear int, sname varchar(10), majorid int)";
    plnr->execute_update(cmd, tx);

    cmd = "insert into student (gradyear, sname, majorid) values (2010, 'foo', 1)";
    plnr->execute_update(cmd, tx);

    cmd = "insert into student (gradyear, sname, majorid) values (2011, 'foo', 1)";
    plnr->execute_update(cmd, tx);

    cmd = "insert into student (gradyear, sname, majorid) values (2010, 'fuga', 30)";
    plnr->execute_update(cmd, tx);

    cmd = "insert into student (gradyear, sname, majorid) values (2010, 'bar', 30)";
    plnr->execute_update(cmd, tx);

    cmd = "insert into student (gradyear, sname, majorid) values (2010, 'fuga', 1)";
    plnr->execute_update(cmd, tx);

    cmd = "insert into student (gradyear, sname, majorid) values (2010, 'fuga', 50)";
    plnr->execute_update(cmd, tx);

    cmd = "update student set sname = 'new-foo' where gradyear = 2010 and majorid = 1";
    plnr->execute_update(cmd, tx);

    // part1 process a query
    std::string qry = "select sname, gradyear from student";
    std::shared_ptr<plan> p = plnr->create_query_plan(qry, tx);
    std::shared_ptr<scan> s = p->open();
    while (s->next()) {
      std::cout << s->get_string("sname") <<  " " << s->get_int("gradyear") << std::endl;
    }
    s->close();

    // part2 process an update command
    cmd = "delete from student where MajorId = 30";
    int num = plnr->execute_update(cmd, tx);
    std::cout << num << " students were deleted" << std::endl;

    
    qry = "select sname, gradyear, majorid from student where majorid = 1";

    p = plnr->create_query_plan(qry, tx);
    s = p->open();
    while (s->next()) {
      std::cout << s->get_string("sname") <<  " " << s->get_int("gradyear") << " " << s->get_int("majorid") << std::endl;
    }
    s->close();
    
    tx->commit();
    fs::remove_all(fs::current_path() / "studentdb");
  }
}
