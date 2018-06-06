
#include "TangoTree.h"
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
TangoTree::TangoTree(const std::vector<double>& weights) {
    //cout << "Building tree for " << weights.size() << " elements"<< endl;
    root = treeFor(0, weights.size());
}

/**
 * Constructs a perfectly balanced tree for the values in the range
 * [low, high).
 */
TangoTree::Node* TangoTree::treeFor(size_t low, size_t high) {
    // TODO : FIX with a constructor for Node
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
TangoTree::~TangoTree() = default;

/**
 * Determines whether the specified key is present in the splay tree. Your
 * implementation should use only O(1) memory. We recommend looking up the
 * top-down splaying approach described in Sleator and Tarjan's paper,
 * tracing through it, and coding it up.
 */
bool TangoTree::contains(size_t key) const {
    Node* curr = root;

    while (curr != NULL) {
        if (key < curr->key) {
            curr = curr->left;
        } else if (key > curr->key) {
            curr = curr->right;
        } else {
            break;
        }

        if (curr->marked)
            curr = cutAndJoin(curr);
    }

    Node *top_path = curr;
    while (!top_path->marked) {
        top_path = top_path->parent;
    }
    Node *node = cut(top_path, curr->depth);

    Node *pred = findMarkedPredecessor(node, curr->key);

    if (pred) {
        join(node, pred, curr->depth);
    }

    return curr and (curr->key == key);
}

TangoTree::Node* TangoTree::cutAndJoin(TangoTree::Node *node) const {
    Node *top_path = node->parent;
    while (!top_path->marked) {
        top_path = top_path->parent;
    }

    int cut_depth = node->min_depth - 1;
    top_path = cut(top_path, cut_depth);
    top_path = join(top_path, node, cut_depth);

    return top_path;
}

TangoTree::Node* TangoTree::cut(TangoTree::Node *v_root, int depth) const {
    Node* l = findMinWithDepth(v_root, depth);
    Node* r = findMaxWithDepth(v_root, depth);

    Node* lp = l ? getPredecessorByNode(l) : NULL;
    Node* rp = r ? getSuccessorByNode(r) : NULL;

    Node* n_root = NULL;

    if (!lp and !rp) {
        n_root = v_root;
    } else if (!rp) {
        split(lp, v_root);
        lp->right->marked = true;
        updateMinMaxPath(lp);
        n_root = merge(lp);
    } else if (!lp) {
        split(rp, v_root);
        rp->left->marked = true;
        updateMinMaxPath(rp);
        n_root = merge(rp);
    } else {
        split(lp, v_root);
        split(rp, lp->right);

        rp->left->marked = true;
        updateMinMaxPath(rp);

        merge(rp);
        n_root = merge(lp);
    }

    return n_root;
}

TangoTree::Node* TangoTree::join(TangoTree::Node *top_path, TangoTree::Node *n, int depth) const {
    Node* n_root = NULL;

    Node* lp = NULL;
    Node* rp = NULL;

    Node* curr = top_path;
    while (curr != n) {
        if (curr->key > n->key) {
            rp = curr;
            curr = curr->left;
        } else {
            lp = curr;
            curr = curr->right;
        }
    }

    if (!rp) {
        split(lp, top_path);
        lp->right->marked = false;
        updateMinMaxPath(lp->right);
        n_root = merge(lp);
    } else if (!lp) {
        split(rp, top_path);
        rp->left->marked = false;
        updateMinMaxPath(rp->left);
        n_root = merge(rp);
    } else {
        split(lp, top_path);
        split(rp, lp->right);
        rp->left->marked = false;
        updateMinMaxPath(rp->left);
        merge(rp);
        n_root = merge(lp);
    }

    return n_root;
}

TangoTree::Node* TangoTree::findMinWithDepth(TangoTree::Node *n, int depth) const {
    while (true) {
        Node *nl = n->left, *nr = n->right;
        if (!isLeaf(nl) and nl->max_depth > depth) {
            n = nl;
        } else if (n->depth > depth) {
            return n;
        } else if (!isLeaf(nr)) {
            n = nr;
        } else {
            return NULL;
        }
    }
}

TangoTree::Node* TangoTree::findMaxWithDepth(TangoTree::Node *n, int depth) const {
    while (true) {
        Node *nl = n->left, *nr = n->right;
        if (!isLeaf(nr) and nr->max_depth > depth) {
            n = nr;
        } else if (n->depth > depth) {
            return n;
        } else if (!isLeaf(nl)) {
            n = nl;
        } else {
            return NULL;
        }
    }
}

TangoTree::Node* TangoTree::findMarkedPredecessor(TangoTree::Node *node, size_t key) const {
    Node *curr = node;

    key--;
    while (curr and curr->key) {
        if (key < curr->key) {
            curr = curr->left;
        } else if (key > curr->key) {
            curr = curr->right;
        } else {
            return NULL;
        }

        if (curr->marked) {
            return curr;
        }
    }

    return NULL;
}

void TangoTree::updateMinMaxPath(TangoTree::Node *n) const {
    // updateMinMax(n);

    while (!n->parent) {
        n = n->parent;
        updateMinMaxPath(n);
    }
}
