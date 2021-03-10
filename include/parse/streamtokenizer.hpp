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
  void comment_char(int pCh);
  void eol_is_significant(bool pFlag);
  int line_no();
  void lower_case_mode(bool pFlag);
  int next_token();
  void ordinary_char(int pCh);
  void ordinary_chars(int pLow, int pHi);
  void parse_numbers();
  void push_back();
  void quote_char(int pCh);
  void reset_syntax();
  void slash_slash_comments(bool pFlag);
  void slash_star_comments(bool pFlag);
  std::string to_string();
  void whitespace_chars(int pLow, int pHi);
  void word_chars(int pLow, int pHi);

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

  bool is_whitespace(int pCh);
  bool is_alphabetic(int pCh);
  bool is_numeric(int pCh);
  bool is_quote(int pCh);
  bool is_comment(int pCh);

  void reset_char(int pCh);
};
} // namespace simpledb
