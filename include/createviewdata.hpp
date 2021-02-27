#pragma once

#include <string>
#include <memory>

#include "querydata.hpp"
#include "object.hpp"

namespace smartdb {
  class create_view_data: public object {
  public:
    int op() override;
    create_view_data();
    create_view_data(const create_view_data &pCVD);
    create_view_data(const std::string &pViewName, const query_data &pQD);
    create_view_data& operator=(const create_view_data &pCVD);
    std::string view_name() const;
    std::string view_def() const;

  private:
    std::string mViewName;
    query_data mQD;
  };
}
