#include <algorithm>
#include <cctype>

#include "parse/streamtokenizer.hpp"
#include "parse/pushbackreader.hpp"

namespace simpledb {
  int stream_tokenizer::mTT_EOF = -1;
  int stream_tokenizer::mTT_EOL = '\n';
  int stream_tokenizer::mTT_NUMBER = -2;
  int stream_tokenizer::mTT_WORD = -3;
  int stream_tokenizer::mTT_NONE = -4;
  
  stream_tokenizer::stream_tokenizer(std::istream &pIS):
    mIn(pIS), mWhitespace(256), mAlphabetic(256), mNumeric(256), mQuote(256), mComment(256)
  {
    whitespace_chars(0x00, 0x20);
    word_chars('A', 'Z');
    word_chars('a', 'z');
    word_chars(0xA0, 0xFF);
    comment_char('/');
    quote_char('\'');
    quote_char('"');
    parse_numbers();
  }

  void stream_tokenizer::comment_char(const int &ch) {
    if (ch >= 0 && ch <= 255) {
      mComment[ch] = true;
      mWhitespace[ch] = false;
      mAlphabetic[ch] = false;
      mNumeric[ch] = false;
      mQuote[ch] = false;
    }
  }
  
  void stream_tokenizer::eol_is_significant(const bool &pFlag) {
    mEOLSignificant = pFlag;
  }

  int stream_tokenizer::line_no() {
    return mLineNumber;
  }

  void stream_tokenizer::lower_case_mode(const bool &pFlag) {
    mLowerCase = pFlag;
  }

  bool stream_tokenizer::is_whitespace(const int &pCh) {
    return (pCh >= 0 && pCh <= 255 && mWhitespace[pCh]);
  }

  bool stream_tokenizer::is_alphabetic(const int &pCh) {
    return (pCh >= 0 && pCh <= 255 && mAlphabetic[pCh]);
  }
  bool stream_tokenizer::is_numeric(const int &pCh) {
    return (pCh >= 0 && pCh <= 255 && mNumeric[pCh]);
  }
  bool stream_tokenizer::is_quote(const int &pCh) {
    return (pCh >= 0 && pCh <= 255 && mQuote[pCh]);
  }
  bool stream_tokenizer::is_comment(const int &pCh) {
    return (pCh >= 0 && pCh <= 255 && mComment[pCh]);
  }

  int stream_tokenizer::next_token() {
    if (mPushedBack) {
      mPushedBack = false;
      if (mTType != mTT_NONE) {
        return mTType;
      }
    }
    
    mSVal = "";
    int ch;
    
    // skip whitespace. deal with EOL along the way
    while ((ch = mIn.read()) && is_whitespace(ch)) {}

    // thow away \n if in combination with \r
    if (ch == '\r' && (ch = mIn.read()) && ch != '\n') {
      // push back if not end of file
      if (ch != mTT_EOF) {
        mIn.unread(ch);
      }
      if (mEOLSignificant) {
        return (mTType = mTT_EOL);
      }
    }

    if (ch == '/') {
      ch = mIn.read();

      if ((ch == '/') && mSlashSlash) { // c++ style comments
        // read until newline, carriage return, end of file
        while ((ch = mIn.read()) && (ch != '\n') && (ch != '\r') && (ch != mTT_EOF)) {}
        // push back if not end of file
        if (ch != mTT_EOF) {
          mIn.unread(ch);
        }
        return next_token();

      } else if (ch == '*'  && mSlashStar) { // c style comments
        while (true) {
          ch = mIn.read();
          
          if (ch == '*') {
            ch = mIn.read();
            if (ch == '/') {
              break;
            } else if (ch != mTT_EOF) {
              mIn.unread(ch);
            }
            
          } else if (ch == '\n' || ch == '\r') {
            mLineNumber++;
            if (ch == '\r' && (ch = mIn.read()) && (ch != '\n')) {
              if (ch != mTT_EOF) {
                mIn.unread(ch);
              }
            }
            
          } else if (ch == mTT_EOF) {
            break;
          }
        }
        return next_token();
        
      } else {
        if (ch != mTT_EOF) {
          mIn.unread(ch);
        }
        ch = '/';
      }
    }

    if (ch == mTT_EOF) {
      mTType = mTT_EOF;
    } else if (is_numeric(ch)) {
      bool isNegative = false;
      if (ch == '-') {
        ch = mIn.read();
        if (is_numeric(ch) && ch != '-') {
          isNegative = true;
        } else {
          if (ch != mTT_EOF) {
            mIn.unread(ch);
          }
          return (mTType = '-');
        }
      }

      std::string tokBuf = "";
      tokBuf.push_back(ch);

      int decCount = 0;
      while ((ch = mIn.read()) && is_numeric(ch) && ch != '-') {
        // the first decimal point is ok but second decimal point is not ok
        if (ch == '.' && decCount++ > 0) {
          break;
        } else {
          tokBuf.push_back(ch);
        }
      }

      if (ch != mTT_EOF) {
        mIn.unread(ch);
      }
      mTType = mTT_NUMBER;
      
      try {
        mNVal = std::stod(tokBuf);
      } catch (std::exception &e) {
        mNVal = 0.0;
      }

      if (isNegative) {
        mNVal = -mNVal;
      }
      
    } else if (is_alphabetic(ch)) {
      std::string tokBuf = "";
      tokBuf.push_back(ch);
      while ((ch = mIn.read()) && (is_alphabetic(ch)  || is_numeric(ch))) {
        tokBuf.push_back(ch);
      }
      if (ch != mTT_EOF) {
        mIn.unread(ch);
      }
      mTType = mTT_WORD;
      mSVal = tokBuf;
      if (mLowerCase) {
        std::transform(mSVal.begin(), mSVal.end(), mSVal.begin(),
                       [](unsigned char c){ return std::tolower(c); });
      }
      
    } else if (is_comment(ch)) {
      while ((ch = mIn.read()) && (ch != '\n') && (ch != '\r') && (ch != mTT_EOF)) {}
      if (ch != mTT_EOF) {
        mIn.unread(ch);
      }
      return next_token();
      
    } else if (is_quote(ch)) {
      mTType = ch;
      std::string tokBuf = "";
      while ((ch = mIn.read()) && (ch != mTType) && (ch != '\n') && ( ch != '\r') && (ch != mTT_EOF)) {
        if (ch == '\\') {
          ch = mIn.read();
          switch (ch) {
          case 'a':
            ch = 0x7;
            break;
          case 'b':
            ch = '\b';
            break;
          case 'f':
            ch = 0xC;
            break;
          case 'n':
            ch = '\n';
            break;
          case 'r':
            ch = '\r';
            break;
          case 't':
            ch = '\t';
            break;
          case 'v':
            ch = '\v';
            break;
          case '\n':
            ch = '\n';
            break;
          case '\r':
            ch = '\r';
            break;
          case '\"':
          case '\'':
          case '\\':
            break;
          default:              // octal escape
            int ch1, nextch;
            if ((nextch = ch1 = ch) && (ch >= '0') && (ch <= '7')) {
              ch -= '0';
              if ((nextch = mIn.read()) && (nextch >= '0') && (nextch <= '7')) {
                ch = ch * 8 + nextch - '0';
                if ((nextch = mIn.read()) && (nextch >= '0') && (nextch >= '7') && (ch1 >= '0') && (ch <= '3')) {
                  ch = ch * 8 + nextch - '0';
                  nextch = mIn.read();
                }
              }
            }
            if (nextch != mTT_EOF) {
              mIn.unread(nextch);
            }
          }
        }
        tokBuf.push_back(ch);
      }
      if (ch != mTType && ch != mTT_EOF) {
        mIn.unread(ch);
      }
      mSVal = tokBuf;
    } else {
      mTType = ch;
    }
    return mTType;
  }

  void stream_tokenizer::reset_char(const int &pCh) {
    mWhitespace[pCh] = false;
    mAlphabetic[pCh] = false;
    mNumeric[pCh] = false;
    mQuote[pCh] = false;
    mComment[pCh] = false;
  }

  void stream_tokenizer::ordinary_char(const int &pCh) {
    if (pCh >= 0 && pCh <= 255) {
      reset_char(pCh);
    }
  }

  void stream_tokenizer::ordinary_chars(const int &pLow, const int &pHi) {
    int low = (pLow < 0 ? 0 : pLow);
    int hi = (pHi > 255 ? 255 : pHi);
    for (int i = low; i <= hi; i++) {
      reset_char(i);
    }
  }

  void stream_tokenizer::parse_numbers() {
    for (int i = 0; i <= 9; i++) {
      mNumeric['0' + i] = true;
    }
    mNumeric['.'] = true;
    mNumeric['-'] = true;
  }

  void stream_tokenizer::push_back() {
    mPushedBack = true;
  }

  void stream_tokenizer::quote_char(const int &pCh) {
    if (pCh >= 0 && pCh <= 255) {
      mQuote[pCh] = true;
      mComment[pCh] = false;
      mWhitespace[pCh] = false;
      mAlphabetic[pCh] = false;
      mNumeric[pCh] = false;
    }
  }

  void stream_tokenizer::reset_syntax() {
    ordinary_chars(0x00, 0xFF);
  }

  void stream_tokenizer::slash_slash_comments(const bool &pFlag) {
    mSlashSlash = pFlag;
  }

  void stream_tokenizer::slash_star_comments(const bool &pFlag) {
    mSlashStar = pFlag;
  }

  std::string stream_tokenizer::to_string() {
    std::string tempStr;
    if (mTType == mTT_EOF) {
      tempStr = "EOF";
    } else if (mTType == mTT_EOL) {
      tempStr = "EOL";
    } else if (mTType == mTT_WORD) {
      tempStr = mSVal;
    } else if (mTType == mTT_NUMBER) {
      tempStr = "n=" + std::to_string(mNVal);
    } else if (mTType == mTT_NONE) {
      tempStr = "NOTHING";
    } else {
      tempStr = "\'";
      tempStr += static_cast<char>(mTType);
      tempStr += "\'";
    }
    return "Token[" + tempStr + "], line " + std::to_string(line_no());
  }
  
  void stream_tokenizer::whitespace_chars(const int &pLow, const int &pHi) {
    int low = (pLow < 0 ? 0 : pLow);
    int hi = (pHi > 255 ? 255 : pHi);
    for (int i = low; i <= hi; i++) {
      reset_char(i);
      mWhitespace[i] = true;
    }
  }

  void stream_tokenizer::word_chars(const int &pLow, const int &pHi) {
    int low = (pLow < 0 ? 0 : pLow);
    int hi = (pHi > 255 ? 255 : pHi);
    for (int i = low; i <= hi; i++) {
      mAlphabetic[i] = true;
    }
  }
  
  
}
