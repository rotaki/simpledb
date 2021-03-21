#include <cmath>
#include <limits>

#include "multibuffer/bufferneeds.hpp"

namespace simpledb {
int buffer_needs::best_root(int pAvailable, int pSize) {
  int avail = pAvailable - 2; // reserve a couple
  if (avail <= 1) {
    return 1;
  }
  int k = std::numeric_limits<int>::max();
  double i = 1.0;
  while (k > avail) {
    i++;
    k = std::ceil(std::pow(static_cast<double>(pSize), 1 / i));
  }
  return k;
}

int buffer_needs::best_factor(int pAvailable, int pSize) {
  int avail = pAvailable - 2; // reserve a couple
  if (avail <= 1) {
    return 1;
  }
  int k = pSize;
  double i = 1.0;
  while (k > avail) {
    i++;
    k = std::ceil(pSize / i);
  }
  return k;
}
} // namespace simpledb
