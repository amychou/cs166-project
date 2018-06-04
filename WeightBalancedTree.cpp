#include "WeightBalancedTree.h"
#include <cmath>     // For fabs
#include <algorithm> // For accumulate
#include <iostream>
using namespace std;

namespace { // Private helper functions
  /**
   * Given a range [low, high) and the weights on the items, along with a
   * candidate index of the root, returns the cost of the split associated
   * with choosing the key at that index as the root of the tree built for
   * the range [low, high).
   *
   * TODO: This implementation is unacceptably slow. You will need to find
   * a way to speed this up so that it runs in time O(1).
   *
   * Complexity: O(1)
   *
   * @param low The low end of the range (inclusive).
   * @param high The high end of the range (exclusive).
   * @param weights The weights of the keys.
   * @param index The index of the item that should be placed into the root.
   * @return The cost of making that split.
   */
  double costOf(size_t low, size_t high, const vector<double>& weights,
                size_t index, vector<double>& prefixes) {
    double lhs = prefixes[index]-prefixes[low];
    double rhs = prefixes[high]-prefixes[index+1];

    return fabs(lhs - rhs); // fabs, not abs, since this is a double.
  }

  pair<int, int> getSection(size_t low, size_t high,
      const vector<double>& weights, vector<double>& prefixes) {
   int t = 0;
   while (low + (1<<t) < high) {
     int left = low + (1<<t)-1;
     double leftSplit  = costOf(low, high, weights, left, prefixes);
     double leftSplitNext = costOf(low, high, weights, left+1, prefixes);
     if (leftSplitNext >= leftSplit) return make_pair(low, left+1);

     int right = high - (1<<t);
     double rightSplit = costOf(low, high, weights, right, prefixes);
     double rightSplitNext = costOf(low, high, weights, right-1, prefixes);
     if (rightSplitNext >= rightSplit) return make_pair(right, high);
     t++;
   }
   return make_pair(low, high);
  }
}

/**
 * Constructs a weight-balanced tree for the elements in the range [low, high),
 * using the provided weights.
 *
 * Complexity: O(n)
 *
 * @param weights The weights associated with the keys. (Implicitly, the keys
 *                are the numbers 0, 1, 2, ..., weights.size() - 1.)
 * @param low The low end of the range to build the tree for (inclusive).
 * @param high The high end of the range to build the tree for (exclusive).
 * @return A weight-balanced tree for the given range.
 */
WeightBalancedTree::Node*
WeightBalancedTree::treeFor(const vector<double>& weights,
                            size_t low, size_t high, vector<double>& prefixes) {
  /* Base case 1: if there are no nodes, the desired tree is empty. */
  if (low == high) return nullptr;

  /* Base case 2: if there is just one node, make the tree for that
   * node by itself.
   */
  if (low + 1 == high) {
    return new Node { low, nullptr, nullptr };
  }

  /* Recursive case: binary search in the appropriate range to find the
   * element that goes into the root.
   *
   * TODO: Right now, this just does a regular, old-fashioned binary search
   * over the range. You proved on the problem set that this will take time
   * Omega(n log n) on some inputs, which isn't fast enough for our purposes.
   * You will need to edit this code along the lines suggested in the problem
   * set to speed it up to O(n).
   *
   * As a hint - the code below is a perfectly correct implementation of
   * binary search. If you were to edit the lhs and rhs bounds to delineate a
   * different range, you'd end up with working code to binary search in just
   * that range!
   */
  pair<int, int> section = getSection(low, high, weights, prefixes);
  size_t lhs = section.first, rhs = section.second;

  /* Now do a proper binary search to find the minimum value. */
  while (lhs + 1 < rhs) {
    /* This isn't the index of the mid that you'd typically see in a binary
     * search. The reason we're doing this is that we need to be able to
     * inspect two consecutive elements. The extra -1 term here is a bias
     * such that if the range has an even number of elements in it, we round
     * down with our probe (ensuring we look at the two most central elements)
     * rather than rounding up (probing two elements off-center.)
     */
    size_t mid = lhs + (rhs - 1 - lhs) / 2;

    /* Look at the goodness of the split at this position and the next. */
    double midSplit  = costOf(low, high, weights, mid, prefixes);
    double nextSplit = costOf(low, high, weights, mid + 1, prefixes);

    /* Throw away half the array. If the values are increasing (or, at least,
     * not decreasing) as we look from the midpoint to the element after it,
     * we know that the optimal split is either at or before the midpoint.
     */
    if (nextSplit >= midSplit) {
      rhs = mid + 1;
    }
    /* Otherwise, the midpoint and everything below it is too early. */
    else {
      lhs = mid + 1;
    }
  }

  /* We're now left with a range of one element. That's the root! */
  return new Node {
    lhs,
    treeFor(weights, low,     lhs, prefixes),
    treeFor(weights, lhs + 1, high, prefixes)
  };
}

/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new, optimally-balanced BST for the
 * elements 0, 1, 2, ..., weights.size() - 1.
 *
 * Notice that the keys are not given to you explicitly, but the assumption
 * is that the keys are 0, 1, 2, ..., weights.size() - 1 with weights
 * weights[0], weights[1], weights[2], ..., weights[weights.size() - 1].
 */
WeightBalancedTree::WeightBalancedTree(const std::vector<double>& weights) {
  std::vector<double> prefixes;
  prefixes.push_back(0);
  for (size_t i = 0; i < weights.size(); i++) prefixes.push_back(prefixes[i] + weights[i]);
  root = treeFor(weights, 0, weights.size(), prefixes);
}

/**
 * Frees all memory used by this tree. You don't need to edit this code.
 */
WeightBalancedTree::~WeightBalancedTree() {
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
 * Determines whether the specified key is present in the weight-balanced
 * tree. You don't need to edit this function.
 *
 * @param key The key to look up.
 * @return Whether it's present in the tree.
 */
bool WeightBalancedTree::contains(size_t key) const {
  Node* curr = root;
  while (true) {
    if (curr == nullptr)  return false;
    if (curr->key == key) return true;

    curr = (key < curr->key? curr->left : curr->right);
  }
}
