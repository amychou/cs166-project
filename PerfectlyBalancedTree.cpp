#include "PerfectlyBalancedTree.h"
using namespace std;

/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new perfectly-balanced tree holding
 * those elements.
 *
 * Because perfectly-balanced trees don't care about access probabilities,
 * w ignore the actual weights provided here and just build a perfectly-
 * balanced tree holding the keys 0, 1, 2, ..., weights.size() - 1.
 */
PerfectlyBalancedTree::PerfectlyBalancedTree(const std::vector<double>& weights) {
  root = treeFor(0, weights.size());
}

/**
 * Constructs a perfectly balanced tree for the values in the range
 * [low, high).
 */
PerfectlyBalancedTree::Node*
PerfectlyBalancedTree::treeFor(size_t low, size_t high) {
  /* Base Case: The empty range is represented by an empty tree. */
  if (low == high) return nullptr;
  
  /* Otherwise, pull out the middle, then recursively construct trees for the
   * left and right ranges.
   */
  size_t mid = low + (high - low) / 2;
  return new Node {
    mid,
    treeFor(low, mid),
    treeFor(mid + 1, high)
  };
}

/**
 * Frees all memory used by this tree.
 */
PerfectlyBalancedTree::~PerfectlyBalancedTree() {
  /* This is the same algorithm we used in PS3. It takes time O(n) and uses
   * only O(1) auxiliary memory.
   */
  while (root != nullptr) {
    /* If the root has no left subtree, just delete the root. */
    if (root->left == nullptr) {
      Node* next = root->right;
      delete root;
      root = next;
    }
    /* Otherwise, the root has a left subtree. Do a right rotation to move
     * that child to the left.
     */
    else {
      Node* child = root->left;
      root->left = child->right;
      child->right = root;
      root = child;
    }
  }
}

/**
 * Determines whether the specified key is present in the perfectly-balanced
 * tree.
 */
bool PerfectlyBalancedTree::contains(size_t key) const {
  Node* curr = root;
  while (true) {
    if (curr == nullptr)  return false;
    if (curr->key == key) return true;
    
    curr = (key < curr->key? curr->left : curr->right);
  }
}
