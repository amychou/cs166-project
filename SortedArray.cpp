#include "SortedArray.h"
#include <algorithm>
using namespace std;

/* The constructor here is given the access probabilities of the numbers 0, 1,
 * 2, ..., weights.size() - 1, but it can't actually use them because the sorted
 * array is agnostic to access probabilities. We therefore completely ignore
 * the weights and just use the vector for information about what keys to
 * store.
 */
SortedArray::SortedArray(const std::vector<double>& weights) : elems(weights.size()) {
  /* Fill the weights array with the values 0, 1, 2, ..., etc. */
  // iota(elems.begin(), elems.end(), 0u);
  for (int i = 0; i < elems.size(); i++) elems[i] = i;
}

bool SortedArray::contains(size_t key) const {
  return binary_search(elems.begin(), elems.end(), key);
}
