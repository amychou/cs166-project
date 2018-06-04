#include "SplayTree.h"
#include <iostream>
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
  //cout << "Building tree for " << weights.size() << " elements"<< endl;
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

void SplayTree::assemble(Node* curr, Node* L, Node* R, Node* rootL, Node* rootR) {
  if (L != nullptr) L->right = curr->left;
  if (R != nullptr) R->left = curr->right;
  if (rootL != nullptr) curr->left = rootL;
  if (rootR != nullptr) curr->right = rootR;
  root = curr;
}

void SplayTree::rotateRight(Node** curr) {
  Node* X = *curr;
  Node* Y = X->left;
  X->left = Y->right;
  Y->right = X;
  *curr = Y;
}

void SplayTree::rotateLeft(Node** curr) {
  Node* X = *curr;
  Node* Y = X->right;
  X->right = Y->left;
  Y->left = X;
  *curr = Y;
}

void SplayTree::linkRight(Node** curr, Node** R, Node** rootR) {
  if (*R != nullptr) (*R)->left = *curr;
  else *rootR = *curr;
  *R = *curr;
  *curr = (*curr)->left;
  (*R)->left = nullptr;
}

void SplayTree::linkLeft(Node** curr, Node** L, Node** rootL) {
  if (*L != nullptr) (*L)->right = *curr;
  else *rootL = *curr;
  *L = *curr;
  *curr = (*curr)->right;
  (*L)->right = nullptr;
}

/**
 * Determines whether the specified key is present in the splay tree. Your
 * implementation should use only O(1) memory. We recommend looking up the
 * top-down splaying approach described in Sleator and Tarjan's paper,
 * tracing through it, and coding it up.
 */
bool SplayTree::contains(size_t key) {
  Node* L = nullptr;
  Node* R = nullptr;
  Node* rootR = nullptr;
  Node* rootL = nullptr;
  Node* curr = root;
  while (curr != nullptr) {
    if (key < curr->key) {
      if (curr->left == nullptr) {
        assemble(curr, L, R, rootL, rootR);
        return false;
      } else if (key == curr->left->key) { // Zig
        linkRight(&curr, &R, &rootR);
      } else if (key < curr->left->key) { // Zig-Zig
        if (curr->left->left == nullptr) {
          linkRight(&curr, &R, &rootR);
          assemble(curr, L, R, rootL, rootR);
          return false;
        }
        rotateRight(&curr);
        linkRight(&curr, &R, &rootR);
      } else {
        if (curr->left->right == nullptr) {
          linkRight(&curr, &R, &rootR);
          assemble(curr, L, R, rootL, rootR);
          return false;
        }
        linkRight(&curr, &R, &rootR); // Zig-Zag
        linkLeft(&curr, &L, &rootL);
      }
    } else if (key > curr->key) {
      if (curr->right == nullptr) {
        assemble(curr, L, R, rootL, rootR);
        return false;
      } else if (key == curr->right->key) { // Zag
        linkLeft(&curr, &L, &rootL);
      } else if (key > curr->right->key) { // Zag-Zag
        if (curr->right->right == nullptr) {
          linkLeft(&curr, &L, &rootL);
          assemble(curr, L, R, rootL, rootR);
          return false;
        }
        rotateLeft(&curr);
        linkLeft(&curr, &L, &rootL);
      } else { //Zag-Zig
        if (curr->right->left == nullptr) {
          linkLeft(&curr, &L, &rootL);
          assemble(curr, L, R, rootL, rootR);
          return false;
        }
        linkLeft(&curr, &L, &rootL);
        linkRight(&curr, &R, &rootR);
      }
    }
    if (curr->key == key) {
      assemble(curr, L, R, rootL, rootR);
      return true;
    }
  }
  assemble(curr, L, R, rootL, rootR);
  return false;
}
