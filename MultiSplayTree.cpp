#include "MultiSplayTree.h"
#include <iostream>
using namespace std;

void MultiSplayTree::check(MultiSplayTree::Node* r) {
  if (!r) return;
  if (r->p) {
    cout << "Parent of " << r->key << " = " << r->p->key << endl;
  } else {
    cout << "Parent of " << r->key << " = null" << endl;
  }
  check(r->c[0]);
  check(r->c[1]);
}

MultiSplayTree::MultiSplayTree(const vector<double>& weights) {
  root = treeFor(0, weights.size(), 0, true);
  // check(root);
}

MultiSplayTree::~MultiSplayTree() {
  /* This is the same algorithm we used in PS3. It takes time O(n) and uses
   * only O(1) auxiliary memory.
   */
  while (root != nullptr) {
    /* If the root has no left subtree, just delete the root. */
    if (root->c[0] == nullptr) {
      Node* next = root->c[1];
      delete root;
      root = next;
    }
    /* Otherwise, the root has a left subtree. Do a right rotation to move
     * that child to the left.
     */
    else {
      Node* child = root->c[0];
      root->c[0] = child->c[1];
      child->c[1] = root;
      root = child;
    }
  }
}

MultiSplayTree::Node* MultiSplayTree::treeFor(size_t low, size_t high, size_t depth, bool isRoot) {
  if (low == high) return nullptr;

  size_t mid = low + (high - low) / 2;
  Node* p = new Node { mid, depth, depth, isRoot};
  p->c[0] = treeFor(low, mid, depth+1, false);
  p->c[1] = treeFor(mid + 1, high, depth+1, true);
  if (p->c[0]) p->c[0]->p = p;
  if (p->c[1]) p->c[1]->p = p;
  return p;
}

void MultiSplayTree::rotate(Node* x) {
  Node* p = x->p;
  if (p->isRoot) {
    p->isRoot = false;
    x->isRoot = true;
  }
  if (root == p) root = x;
  if (p->p) {
    int d = p->p->c[1]==p;
    p->p->c[d] = x;
  }
  x->p = p->p;
  int d = p->c[1]==x;
  p->c[d] = x->c[!d]; if (x->c[!d]) x->c[!d]->p = p;
  x->c[!d] = p; p->p = x;
  x->minDepth = p->minDepth;
  p->minDepth = p->depth;
  if (p->c[0]) p->minDepth = min(p->minDepth, p->c[0]->minDepth);
  if (p->c[1]) p->minDepth = min(p->minDepth, p->c[1]->minDepth);
}

void MultiSplayTree::splay(Node* x, Node* top = 0) {
  while (!(x->isRoot || x->p==top)) {
    Node* p = x->p;
    if (!(p->isRoot || p->p == top)) {
      Node* g = p->p;
      if ((g->c[0]==p && p->c[0]==x) || (g->c[1]==p && p->c[1]==x)) rotate(p);
      else rotate(x);
    }
    rotate(x);
  }
}

MultiSplayTree::Node* MultiSplayTree::refParent(MultiSplayTree::Node* y, int r) {
  Node* curr = y->c[r];
  while (true) {
    if (curr->c[!r] && curr->c[!r]->minDepth < y->depth) curr = curr->c[!r];
    else if (curr->c[r] && curr->c[r]->minDepth < y->depth) curr = curr->c[r];
    else break;
  }
  return curr;
}

void MultiSplayTree::switchPath(Node* y) {
  if (y->c[0]) {
    if (y->c[0]->minDepth > y->depth) {
      y->c[0]->isRoot = !y->c[0]->isRoot;
    } else {
      Node* x = refParent(y, 0);
      splay(x, y);
      if (x->c[1]) x->c[1]->isRoot = !x->c[1]->isRoot;
    }
  }
  if (y->c[1]) {
    if (y->c[1]->minDepth > y->depth) {
      y->c[1]->isRoot = !y->c[1]->isRoot;
    } else {
      Node* z = refParent(y, 1);
      splay(z, y);
      if (z->c[0]) z->c[0]->isRoot = !z->c[0]->isRoot;
    }
  }
}

void MultiSplayTree::expose(Node* v) {
  Node* curr = v;
  while (curr->p) {
    Node* y = curr->p;
    if (curr->isRoot) {
      splay(y);
      switchPath(y);
    }
    curr = y;
  }
  splay(v);
}

bool MultiSplayTree::contains(std::size_t key) {
  Node* curr = root;
  Node* prev = root;
  while (curr && curr->key != key) {
    prev = curr;
    if (key < curr->key) {
      curr = curr->c[0];
    } else {
      curr = curr->c[1];
    }
  }
  if (!curr) {
    expose(prev);
    return false;
  } else {
    expose(curr);
    return true;
  }
}
