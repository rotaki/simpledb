#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "parse/pushbackreader.hpp"

namespace simpledb {
class stream_tokenizer {
public:
  static int mTT_EOF;
  static int mTT_EOL;
  static int mTT_NUMBER;
  static int mTT_WORD;
  static int mTT_NONE;

  int mTType = mTT_NONE;
  std::string mSVal;
  double mNVal;

  stream_tokenizer(std::istream &pIS);
  void comment_char(const int &pCh);
  void eol_is_significant(const bool &pFlag);
  int line_no();
  void lower_case_mode(const bool &pFlag);
  int next_token();
  void ordinary_char(const int &pCh);
  void ordinary_chars(const int &pLow, const int &pHi);
  void parse_numbers();
  void push_back();
  void quote_char(const int &pCh);
  void reset_syntax();
  void slash_slash_comments(const bool &pFlag);
  void slash_star_comments(const bool &pFlag);
  std::string to_string();
  void whitespace_chars(const int &pLow, const int &pHi);
  void word_chars(const int &pLow, const int &pHi);

private:
  bool mEOLSignificant = false;
  bool mLowerCase = false;
  bool mSlashSlash = false;
  bool mSlashStar = false;
  bool mPushedBack = false;
  int mLineNumber = 1;

  push_back_reader mIn;
  std::vector<bool> mWhitespace;
  std::vector<bool> mAlphabetic;
  std::vector<bool> mNumeric;
  std::vector<bool> mQuote;
  std::vector<bool> mComment;

  bool is_whitespace(const int &pCh);
  bool is_alphabetic(const int &pCh);
  bool is_numeric(const int &pCh);
  bool is_quote(const int &pCh);
  bool is_comment(const int &pCh);

  void reset_char(const int &pCh);
};
} // namespace simpledb
