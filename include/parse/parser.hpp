#pragma once

#include <string>
#include <memory>

#include "parse/lexer.hpp"
#include "parse/querydata.hpp"
#include "parse/insertdata.hpp"
#include "parse/deletedata.hpp"
#include "parse/modifydata.hpp"
#include "parse/createtabledata.hpp"
#include "parse/createviewdata.hpp"
#include "parse/createindexdata.hpp"
#include "parse/object.hpp"
#include "query/constant.hpp"
#include "query/expression.hpp"
#include "query/term.hpp"
#include "query/predicate.hpp"
#include "record/schema.hpp"

namespace smartdb {
  class parser {
  public:
    parser(const std::string &pS);

    // todo is it ok to return pointer?
    std::string new_field();
    constant new_constant();
    expression new_expression();
    term new_term();
    predicate new_predicate();


    // methods for parsing queries
    std::shared_ptr<query_data> query();
    
    // methods for parsing the varioud update commands
    std::shared_ptr<object> update_cmd();

    // methods for parsing remove commands
    std::shared_ptr<delete_data> remove();

    // methods for parsing insert commands
    std::shared_ptr<insert_data> insert();

    // methods for parsing modify commands
    std::shared_ptr<modify_data> modify();

    // method for parsing create table commands
    std::shared_ptr<create_table_data> create_table();

    // method for parsing create view commands
    std::shared_ptr<create_view_data> create_view();

    // method for parsing creaete index commands
    std::shared_ptr<create_index_data> create_index();


  private:
    std::shared_ptr<lexer> lex;
    
    // methods for parsing queries
    std::vector<std::string> select_list();
    std::set<std::string> table_list();

    // methods for parsing the various update commands
    std::shared_ptr<object> create();

    // methods for parsing insert commands
    std::vector<std::string> field_list();
    std::vector<constant> const_list();

    // method for parsing create table commands
    std::shared_ptr<schema> field_defs();
    std::shared_ptr<schema> field_def();
    std::shared_ptr<schema> field_type(const std::string &pFldName);

  };
}
