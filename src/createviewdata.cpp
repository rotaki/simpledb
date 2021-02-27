#include "createviewdata.hpp"

namespace smartdb {
  int create_view_data::op() {
    return createview;
  }
  
  create_view_data::create_view_data() {}

  create_view_data::create_view_data(const create_view_data &pCVD):
    mViewName(pCVD.mViewName), mQD(pCVD.mQD) {}

  create_view_data::create_view_data(const std::string &pViewName, const query_data &pQD):
    mViewName(pViewName), mQD(pQD) {}

  create_view_data& create_view_data::operator=(const create_view_data &pCVD) {
    if (this != &pCVD) {
      mViewName = pCVD.mViewName;
      mQD = pCVD.mQD;
    }
    return *this;
  }

  std::string create_view_data::view_name() const {
    return mViewName;
  }
  
  std::string create_view_data::view_def() const {
    return mQD.to_string();
  }
}
