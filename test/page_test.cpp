#include "page.hpp"

#include "gtest/gtest.h"

namespace smartdb {
  TEST(page, set_int) {
    page p1(sizeof(int));
    p1.set_int(0, 4);
    int num = p1.get_int(0);
    EXPECT_EQ(4, num);

    page p2(sizeof(int)-1);
    try {
      p2.set_int(0, 5);
    } catch (std::runtime_error &e) {
      EXPECT_EQ(e.what(), std::string("Page overflow while set_int"));
    } catch (...) {
      FAIL();
    }
  }
}
