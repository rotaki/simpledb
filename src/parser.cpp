#include "parser.hpp"

namespace smartdb {
  parser::parser(const std::string &pS) {
    lex = std::shared_ptr<lexer>(new lexer(pS));
  }

  std::string parser::new_field() {
    return lex->eat_id();
  }

  constant parser::new_constant() {
    if (lex->match_string_constant()) {
      return constant(lex->eat_string_constant());
    } else {
      return constant(lex->eat_int_constant());
    }
  }

  expression parser::new_expression() {
    if (lex->match_id()) {
      return expression(new_field());
    } else {
      return expression(new_constant());
    }
  }

  term parser::new_term() {
    expression lhs = new_expression();
    lex->eat_delim('=');
    expression rhs = new_expression();
    return term(lhs, rhs);
  }

  predicate parser::new_predicate() {
    predicate pred(new_term());
    if (lex->match_keyword("and")) {
      lex->eat_keyword("and");
      pred.con_join_with(new_predicate());
    }
    return pred;
  }

  std::shared_ptr<query_data> parser::query() {
    lex->eat_keyword("select");
    std::vector<std::string> fields = select_list();
    lex->eat_keyword("from");
    std::set<std::string> tables = table_list();
    predicate pred;
    if (lex->match_keyword("where")) {
      lex->eat_keyword("where");
      pred = new_predicate();
    }
    return std::shared_ptr<query_data>(new query_data(fields, tables, pred));
  }

  std::vector<std::string> parser::select_list() {
    std::vector<std::string> L;
    L.emplace_back(new_field());
    if (lex->match_delim(',')) {
      lex->eat_delim(',');
      std::vector<std::string> LL = select_list();
      L.insert(L.end(), LL.begin(), LL.end());
    }
    return L;
  }

  std::set<std::string> parser::table_list() {
    std::set<std::string> L;
    L.insert(lex->eat_id());
    if (lex->match_delim(',')) {
      lex->eat_delim(',');
      std::set<std::string> LL = table_list();
      L.insert(LL.begin(), LL.end());
    }
    return L;
  }

  // todo udpate_cmd
  std::shared_ptr<object> parser::update_cmd() {
    if (lex->match_keyword("insert")) {
      return insert();
    } else if (lex->match_keyword("delete")) {
      return remove();
    } else if (lex->match_keyword("update")) {
      return modify();
    } else {
      return create();
    }
  }

  std::shared_ptr<object> parser::create() {
    lex->eat_keyword("create");
    if (lex->match_keyword("table")) {
      return create_table();
    } else if (lex->match_keyword("view")){
      return create_view();
    } else {
      return create_index();
    }
  }

  std::shared_ptr<delete_data> parser::remove() {
    lex->eat_keyword("delete");
    lex->eat_keyword("from");
    std::string tblName = lex->eat_id();
    predicate pred;
    if (lex->match_keyword("where")) {
      lex->eat_keyword("where");
      pred = new_predicate();
    }
    return std::shared_ptr<delete_data>(new delete_data(tblName, pred));
  }

  std::shared_ptr<insert_data> parser::insert() {
    lex->eat_keyword("insert");
    lex->eat_keyword("into");
    std::string tblName = lex->eat_id();
    lex->eat_delim('(');
    std::vector<std::string> flds = field_list();
    lex->eat_delim(')');
    lex->eat_keyword("values");
    lex->eat_delim('(');
    std::vector<constant> vals = const_list();
    lex->eat_delim(')');
    return std::shared_ptr<insert_data>(new insert_data(tblName, flds, vals));
  }

  std::vector<std::string> parser::field_list() {
    std::vector<std::string> L;
    L.emplace_back(new_field());
    if (lex->match_delim(',')) {
      lex->eat_delim(',');
      std::vector<std::string> LL = field_list();
      L.insert(L.end(), LL.begin(), LL.end());
    }
    return L;
  }

  std::vector<constant> parser::const_list() {
    std::vector<constant> L;
    L.emplace_back(new_constant());
    if (lex->match_delim(',')) {
      lex->eat_delim(',');
      std::vector<constant> LL = const_list();
      L.insert(L.end(), LL.begin(), LL.end());
    }
    return L;
  }

  std::shared_ptr<modify_data> parser::modify() {
    lex->eat_keyword("update");
    std::string tblName = lex->eat_id();
    lex->eat_keyword("set");
    std::string fldName = new_field();
    lex->eat_delim('=');
    expression newVal = new_expression();
    predicate pred;
    if (lex->match_keyword("where")) {
      lex->eat_keyword("where");
      pred = new_predicate();
    }
    return std::shared_ptr<modify_data>(new modify_data(tblName, fldName, newVal, pred));
  }

  std::shared_ptr<create_table_data> parser::create_table() {
    lex->eat_keyword("table");
    std::string tblName = lex->eat_id();
    lex->eat_delim('(');
    std::shared_ptr<schema> sch = field_defs();
    lex->eat_delim(')');
    return std::shared_ptr<create_table_data>(new create_table_data(tblName, sch));
  }

  std::shared_ptr<schema> parser::field_defs() {
    std::shared_ptr<schema> sch = field_def();
    if (lex->match_delim(',')) {
      lex->eat_delim(',');
      std::shared_ptr<schema> sch2 = field_defs();
      sch->add_all(sch2);
    }
    return sch;
  }

  std::shared_ptr<schema> parser::field_def() {
    std::string fldName = new_field();
    return field_type(fldName);
  }

  std::shared_ptr<schema> parser::field_type(const std::string &pFldName) {
    std::shared_ptr<schema> sch(new schema);
    if (lex->match_keyword("int")) {
      lex->eat_keyword("int");
      sch->add_int_field(pFldName);
    } else {
      lex->eat_keyword("varchar");
      lex->eat_delim('(');
      int strLen = lex->eat_int_constant();
      lex->eat_delim(')');
      sch->add_string_field(pFldName, strLen);
    }
    return sch;
  }

  std::shared_ptr<create_view_data> parser::create_view() {
    lex->eat_keyword("view");
    std::string viewName = lex->eat_id();
    lex->eat_keyword("as");
    std::shared_ptr<query_data> qD = query();
    return std::shared_ptr<create_view_data>(new create_view_data(viewName, qD));
  }

  std::shared_ptr<create_index_data> parser::create_index() {
    lex->eat_keyword("index");
    std::string idxName = lex->eat_id();
    lex->eat_keyword("on");
    std::string tblName = lex->eat_id();
    lex->eat_delim('(');
    std::string fldName = new_field();
    lex->eat_delim(')');
    return std::shared_ptr<create_index_data>(new create_index_data(idxName, tblName, fldName));
  }
}
