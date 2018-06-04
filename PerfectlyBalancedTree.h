#ifndef PerfectlyBalancedTree_Included
#define PerfectlyBalancedTree_Included

#include <string>
#include <vector>

class PerfectlyBalancedTree {
public:
  /**
   * Given a list of the future access probabilities of the elements 0, 1, 2,
   * ..., weights.size() - 1, constructs a new, optimally-balanced BST for the
   * elements 0, 1, 2, ..., weights.size() - 1.
   *
   * Because perfectly balanced trees have to be balanced without regard to the
   * access probabilities of the underlying elements, you should ignore the 
   * assigned probabilities here and just build an optimally-balanced BST for
   * the elements 0, 1, 2, ..., weights.size() - 1 however you'd like.
   *
   * Complexity: O(n)
   */
  explicit PerfectlyBalancedTree(const std::vector<double>& weights);
  
  /**
   * Cleans up all memory allocated by the tree. Remember that destructors are
   * invoked automatically in C++, so you should never need to directly invoke
   * this member function.
   */
  ~PerfectlyBalancedTree();
  
  /**
   * Searches the tree for the given key, returning whether or not that key is
   * present in the tree.
   */
  bool contains(std::size_t key) const;
  
  /**
   * Returns a human-readable name of this type.
   */
  static std::string name() {
    return "Perfectly Balanced Tree";
  }
  
private:
  /* Type representing a node in the tree. */
  struct Node {
    std::size_t key;
    Node* left;
    Node* right;
  };
  
  /* Pointer to the root. */
  Node* root;
  
  /* Helper function to construct a perfectly-balanced tree. */
  static Node* treeFor(std::size_t low, std::size_t high);
  
  /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  PerfectlyBalancedTree(PerfectlyBalancedTree const &) = delete;
  void operator=(PerfectlyBalancedTree const &) = delete;
};

#endif
