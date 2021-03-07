#pragma once

#include <memory>
#include <string>

#include "parse/object.hpp"
#include "parse/querydata.hpp"

namespace simpledb {
class create_view_data : public object {
public:
  int op() override;
  create_view_data(const std::string &pViewName,
                   std::unique_ptr<query_data> pQD);
  std::string view_name() const;
  std::string view_def() const;

private:
  std::string mViewName;
  std::unique_ptr<query_data> mQD;
};
} // namespace simpledb
