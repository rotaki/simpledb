#include "parse/createviewdata.hpp"

namespace smartdb {
  int create_view_data::op() {
    return createview;
  }
  
  create_view_data::create_view_data(const std::string &pViewName, std::unique_ptr<query_data> pQD):
    mViewName(pViewName), mQD(std::move(pQD)) {}

  std::string create_view_data::view_name() const {
    return mViewName;
  }
  
  std::string create_view_data::view_def() const {
    return mQD->to_string();
  }
}
