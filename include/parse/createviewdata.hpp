#pragma once

#include <string>
#include <memory>

#include "parse/querydata.hpp"
#include "parse/object.hpp"

namespace smartdb {
  class create_view_data: public object {
  public:
    int op() override;
    create_view_data(const std::string &pViewName, std::unique_ptr<query_data> pQD);
    std::string view_name() const;
    std::string view_def() const;

  private:
    std::string mViewName;
    std::unique_ptr<query_data> mQD;
  };
}
