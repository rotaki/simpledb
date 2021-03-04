#pragma once

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
}
