#pragma once
#include <ostream>

namespace smartdb {
  class object {
  public:
    enum operation {
                    insert,
                    remove,
                    modify,
                    createtable,
                    createview,
                    createindex
    };
    virtual ~object() {};
    virtual int op() = 0;
  };

  inline std::ostream& operator<<(std::ostream &os, const object::operation &op) {
    switch (op) {
    case object::insert:
      os << "insert";
      break;
    case object::remove:
      os << "remove";
      break;
    case object::modify:
      os << "modify";
      break;
    case object::createtable:
      os << "createtable";
      break;
    case object::createview:
      os << "createview";
      break;
    case object::createindex:
      os << "createindex";
      break;
    default:
      break;
    }
    return os;
  }
}
