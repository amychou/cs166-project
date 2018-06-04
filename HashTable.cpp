#include "HashTable.h"
using namespace std;

/* The constructor here is given the access probabilities of the numbers 0, 1,
 * 2, ..., weights.size() - 1, but it can't actually use them because the hash
 * table is agnostic to access probabilities. We therefore completely ignore
 * the weights and just use the vector for information about what keys to
 * store in the hash table.
 */
HashTable::HashTable(const std::vector<double>& weights) {
  for (size_t i = 0; i < weights.size(); i++) {
    elems.insert(i);
  }
}

bool HashTable::contains(size_t key) const {
  return elems.find(key) != elems.end();
}
