#include "SplayTree.h"
#include <cassert>

using namespace std;

/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new splay tree holding those
 * elements.
 *
 * Because splay trees rearrange their elements in response to queries, you
 * can safely ignore the assigned probabilities here and just build a BST
 * storing the elements 0, 1, 2, ..., weights.size() - 1 however you'd like.
 */
SplayTree::SplayTree(const std::vector<double>& weights) {
  root = treeFor(0, weights.size());
}

/**
 * Constructs a perfectly balanced tree for the values in the range
 * [low, high).
 */
SplayTree::Node*
SplayTree::treeFor(size_t low, size_t high) {
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
SplayTree::~SplayTree() {
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

void SplayTree::rotate_left() const {
  Node *temp = root->right;
  root->right = temp->left;
  temp->left = root;
  root = temp;
}

void SplayTree::rotate_right() const {
  Node *temp = root->left;
  root->left = temp->right;
  temp->right = root;
  root = temp;
}

void SplayTree::link_left() const {
  left->right = root;
  left = root;
  root = root->right;
}

void SplayTree::link_right() const {
  right->left = root;
  right = root;
  root = root->left;
}

/**
 * Determines whether the specified key is present in the splay tree. Your
 * implementation should use only O(1) memory. We recommend looking up the
 * top-down splaying approach described in Sleator and Tarjan's paper,
 * tracing through it, and coding it up.
 */
bool SplayTree::contains(size_t key) const {
  Node aux_roots; // aux_roots stores the roots of trees L and R

  left = right = &aux_roots;
  // left stores bottom right of L, right stores bottom left of R

  aux_roots.left = aux_roots.right = NULL;
  // aux_roots.left will end up storing the root of R and vice-versa

  bool found = false;
  while (true) {
    if (key < root->key) {
      if (root->left == NULL) break;

      if (key < root->left->key) {
        rotate_right(); // zig-zig encountered, rotate right first
        if (root->left == NULL) break;
      }

      link_right();

      if (key > root->key) {
        if (root->right == NULL) break;

        // zig-zag encountered, link left as well
        link_left();
      }
    } else if (root->key < key) {
      if (root->right == NULL) break;

      if (root->right->key < key) {
        rotate_left(); // zig-zig encountered, rotate left first
        if (root->right == NULL) break;
      }

      link_left();

      if (key < root->key) {
        if (root->left == NULL) break;

        // zig-zag encountered, link right as well
        link_right();
      }
    } else {
      found = true;
      break;
    }
  }

  // assemble
  left->right = root->left;
  right->left = root->right;
  root->left = aux_roots.right;
  root->right = aux_roots.left;

  return found;
}