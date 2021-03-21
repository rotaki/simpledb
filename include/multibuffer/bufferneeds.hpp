#pragma once

namespace simpledb {
class buffer_needs {
public:
  static int best_root(int pAvailable, int pSize);
  static int best_factor(int pAvailable, int pSize);
};
} // namespace simpledb
