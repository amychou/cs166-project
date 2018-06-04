#ifndef SortedArray_Included
#define SortedArray_Included

#include <vector>
#include <string>

/**
 * This implementation is not a BST. It's backed by a std::vector containing
 * the keys in sorted order. It's just for reference, to show off the cost
 * of making things dynamic.
 */
class SortedArray {
public:
  /**
   * Given a list of the future access probabilities of the elements 0, 1, 2,
   * ..., weights.size() - 1, constructs a new std::unordered_set-backed
   * structure for the elements 0, 1, 2, ..., weights.size() - 1. We're
   * completely ignoring access probabilites and just building a list of
   * keys, so the weights don't matter.
   */
  explicit SortedArray(const std::vector<double>& weights);
  
  /**
   * Returns whether the given key is present in the sorted array.
   */
  bool contains(std::size_t key) const;
  
  /**
   * Returns a human-readable name of this type.
   */
  static std::string name() {
    return "Sorted Array";
  }
  
private:
  std::vector<std::size_t> elems; // The actual elements
  
  /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  SortedArray(SortedArray const &) = delete;
  void operator=(SortedArray const &) = delete;
};

#endif
