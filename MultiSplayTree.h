#ifndef MultiSplayTree_Included
#define MultiSplayTree_Included

#include <string>
#include <vector>

/**
 * A type representing a binary search tree backed by a splay tree.
 */
class MultiSplayTree {
public:
  /**
   * Given a list of the future access probabilities of the elements 0, 1, 2,
   * ..., weights.size() - 1, constructs a new splay tree holding those
   * elements.
   *
   * Because splay trees rearrange their elements in response to queries, we
   * ignore the assigned probabilities here and just build a BST storing the
   * keys 0, 1, 2, ..., weights.size() - 1.
   */
  explicit MultiSplayTree(const std::vector<double>& weights);

  /**
   * Cleans up all memory allocated by the tree. Remember that destructors are
   * invoked automatically in C++, so you should never need to directly invoke
   * this member function.
   */
  ~MultiSplayTree();

  /**
   * Searches the splay tree for the given key, returning whether or not that
   * key is present in the tree. We strongly recommend implementing this
   * method using the top-down splaying technique outlined in Sleator and
   * Tarjan's original paper.
   */
  bool contains(std::size_t key); // bool contains(std::size_t key) const;

  /**
   * Returns a human-readable name of this type.
   */
  static std::string name() {
    return "Multi-Splay Tree";
  }

private:
  struct Node {
    std::size_t key;
    Node *c[2], *p;
    size_t depth;
    size_t minDepth;
    bool isRoot;
    Node(size_t key, size_t depth=0, size_t minDepth=0, bool isRoot=0) {
      this->key = key;
      c[0] = c[1] = p = 0;
      this->isRoot = isRoot;
      this->depth = depth;
      this->minDepth = minDepth;
    }
    /* Note: Since you'll be implementing top-down splaying, you should *not*
     * add a parent pointer here. You won't be needing it.
     */
  };

  /* Pointer to the root of the tree - which will likely change a lot!
   *
   * This variable is marked mutable. The mutable keyword in C++ means that
   * this data member can be modified by const member functions. As a result,
   * even though the contains() member function is const, it's still allowed
   * to change this value.
   *
   * The mutable keyword, more generally, is used in contexts where you have an
   * operation that doesn't logically change the internal state of an object,
   * but which might change the actual bits of that object.
   */
  mutable Node* root;

  /* Constructs an initial tree for all the keys in the given range. This is
   * taken verbatim from the perfectly-balanced tree.
   */
  static Node* treeFor(std::size_t low, std::size_t high, std::size_t depth, bool isRoot);

  // TODO: Add any necessary new types or fields here.
  void rotate(Node* x);
  void splay(Node* x, Node* top);
  Node* refParent(Node*y, int r);
  void switchPath(Node* y);
  void expose(Node* v);
  void check(Node* r);


  /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  MultiSplayTree(MultiSplayTree const &) = delete;
  void operator=(MultiSplayTree const &) = delete;
};

#endif
