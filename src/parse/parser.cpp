#include "parse/parser.hpp"

namespace simpledb {
parser::parser(const std::string &pS) : lex(pS) {}

std::string parser::new_field() { return lex.eat_id(); }

constant parser::new_constant() {
  if (lex.match_string_constant()) {
    return constant(lex.eat_string_constant());
  } else {
    return constant(lex.eat_int_constant());
  }
}

expression parser::new_expression() {
  if (lex.match_id()) {
    return expression(new_field());
  } else {
    return expression(new_constant());
  }
}

term parser::new_term() {
  expression lhs = new_expression();
  lex.eat_delim('=');
  expression rhs = new_expression();
  return term(lhs, rhs);
}

predicate parser::new_predicate() {
  predicate pred(new_term());
  if (lex.match_keyword("and")) {
    lex.eat_keyword("and");
    pred.con_join_with(new_predicate());
  }
  return pred;
}

std::unique_ptr<query_data> parser::query() {
  lex.eat_keyword("select");
  std::vector<std::string> fields = select_list();
  lex.eat_keyword("from");
  std::set<std::string> tables = table_list();
  predicate pred;
  if (lex.match_keyword("where")) {
    lex.eat_keyword("where");
    pred = new_predicate();
  }
  return std::make_unique<query_data>(fields, tables, pred);
}

std::vector<std::string> parser::select_list() {
  std::vector<std::string> L;
  L.emplace_back(new_field());
  if (lex.match_delim(',')) {
    lex.eat_delim(',');
    std::vector<std::string> LL = select_list();
    L.insert(L.end(), LL.begin(), LL.end());
  }
  return L;
}

std::set<std::string> parser::table_list() {
  std::set<std::string> L;
  L.insert(lex.eat_id());
  if (lex.match_delim(',')) {
    lex.eat_delim(',');
    std::set<std::string> LL = table_list();
    L.insert(LL.begin(), LL.end());
  }
  return L;
}

// todo udpate_cmd
std::unique_ptr<object> parser::update_cmd() {
  if (lex.match_keyword("insert")) {
    return static_unique_pointer_cast<object>(insert());
  } else if (lex.match_keyword("delete")) {
    return static_unique_pointer_cast<object>(remove());
  } else if (lex.match_keyword("update")) {
    return static_unique_pointer_cast<object>(modify());
  } else {
    return create();
  }
}

std::unique_ptr<object> parser::create() {
  lex.eat_keyword("create");
  if (lex.match_keyword("table")) {
    return static_unique_pointer_cast<object>(create_table());
  } else if (lex.match_keyword("view")) {
    return static_unique_pointer_cast<object>(create_view());
  } else {
    return static_unique_pointer_cast<object>(create_index());
  }
}

std::unique_ptr<delete_data> parser::remove() {
  lex.eat_keyword("delete");
  lex.eat_keyword("from");
  std::string tblName = lex.eat_id();
  predicate pred;
  if (lex.match_keyword("where")) {
    lex.eat_keyword("where");
    pred = new_predicate();
  }
  return std::make_unique<delete_data>(tblName, pred);
}

std::unique_ptr<insert_data> parser::insert() {
  lex.eat_keyword("insert");
  lex.eat_keyword("into");
  std::string tblName = lex.eat_id();
  lex.eat_delim('(');
  std::vector<std::string> flds = field_list();
  lex.eat_delim(')');
  lex.eat_keyword("values");
  lex.eat_delim('(');
  std::vector<constant> vals = const_list();
  lex.eat_delim(')');
  return std::make_unique<insert_data>(tblName, flds, vals);
}

std::vector<std::string> parser::field_list() {
  std::vector<std::string> L;
  L.emplace_back(new_field());
  if (lex.match_delim(',')) {
    lex.eat_delim(',');
    std::vector<std::string> LL = field_list();
    L.insert(L.end(), LL.begin(), LL.end());
  }
  return L;
}

std::vector<constant> parser::const_list() {
  std::vector<constant> L;
  L.emplace_back(new_constant());
  if (lex.match_delim(',')) {
    lex.eat_delim(',');
    std::vector<constant> LL = const_list();
    L.insert(L.end(), LL.begin(), LL.end());
  }
  return L;
}

std::unique_ptr<modify_data> parser::modify() {
  lex.eat_keyword("update");
  std::string tblName = lex.eat_id();
  lex.eat_keyword("set");
  std::string fldName = new_field();
  lex.eat_delim('=');
  expression newVal = new_expression();
  predicate pred;
  if (lex.match_keyword("where")) {
    lex.eat_keyword("where");
    pred = new_predicate();
  }
  return std::make_unique<modify_data>(tblName, fldName, newVal, pred);
}

std::unique_ptr<create_table_data> parser::create_table() {
  lex.eat_keyword("table");
  std::string tblName = lex.eat_id();
  lex.eat_delim('(');
  schema sch = field_defs();
  lex.eat_delim(')');
  return std::make_unique<create_table_data>(tblName, sch);
}

schema parser::field_defs() {
  schema sch = field_def();
  if (lex.match_delim(',')) {
    lex.eat_delim(',');
    sch.add_all(field_defs());
  }
  return sch;
}

schema parser::field_def() {
  std::string fldName = new_field();
  return field_type(fldName);
}

schema parser::field_type(const std::string &pFldName) {
  schema sch;
  if (lex.match_keyword("int")) {
    lex.eat_keyword("int");
    sch.add_int_field(pFldName);
  } else {
    lex.eat_keyword("varchar");
    lex.eat_delim('(');
    int strLen = lex.eat_int_constant();
    lex.eat_delim(')');
    sch.add_string_field(pFldName, strLen);
  }
  return sch;
}

std::unique_ptr<create_view_data> parser::create_view() {
  lex.eat_keyword("view");
  std::string viewName = lex.eat_id();
  lex.eat_keyword("as");
  std::unique_ptr<query_data> qD = query();
  return std::make_unique<create_view_data>(viewName, std::move(qD));
}

std::unique_ptr<create_index_data> parser::create_index() {
  lex.eat_keyword("index");
  std::string idxName = lex.eat_id();
  lex.eat_keyword("on");
  std::string tblName = lex.eat_id();
  lex.eat_delim('(');
  std::string fldName = new_field();
  lex.eat_delim(')');
  return std::make_unique<create_index_data>(idxName, tblName, fldName);
}
} // namespace simpledb
