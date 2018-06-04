#ifndef StdSetTree_Included
#define StdSetTree_Included

#include <set>
#include <string>
#include <vector>

/**
 * A BST type backed by the std::set container, which is typically implemented
 * as a red/black tree. This is designed as a comparison point against which
 * we'll measure the weight-balanced and splay trees. It's not really a fair
 * comparison because the std::set has to handle all sorts of cases that aren't
 * actually going to come up in our tests, but it's still an interesting
 * reference point. We've also included this so that you can get a sense for
 * what the constructor and contains functions should do.
 */
class StdSetTree {
public:
  /**
   * Given a list of the future access probabilities of the elements 0, 1, 2,
   * ..., weights.size() - 1, constructs a new std::set-backed tree holding
   * the elements 0, 1, 2, ..., weights.size() - 1. Because std::set doesn't
   * take access probabilities into account, we'll completely ignore the
   * provided weights and just build a std::set holding 0, 1, 2, ...,
   * weights.size() - 1.
   */
  explicit StdSetTree(const std::vector<double>& weights);
  
  /**
   * Returns whether the given key is present in the tree.
   */
  bool contains(std::size_t key) const;
  
  /**
   * Returns a human-readable name of this type.
   */
  static std::string name() {
    return "std::set";
  }

private:
  std::set<std::size_t> elems; // The actual elements

  /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  StdSetTree(StdSetTree const &) = delete;
  void operator=(StdSetTree const &) = delete;
};

#endif
