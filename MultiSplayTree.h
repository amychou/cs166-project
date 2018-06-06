#ifndef MultiSplayTree_Included
#define MultiSplayTree_Included

#include <string>
#include <vector>


class MultiSplayTree {
public:
  explicit MultiSplayTree(const std::vector<double>& weights);

  ~MultiSplayTree();

  bool contains(std::size_t key); // bool contains(std::size_t key) const;

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
  };

  mutable Node* root;

  static Node* treeFor(std::size_t low, std::size_t high, std::size_t depth, bool isRoot);
  void rotate(Node* x);
  void splay(Node* x, Node* top);
  Node* refParent(Node*y, int r);
  void switchPath(Node* y);
  void expose(Node* v);
  void check(Node* r);
  MultiSplayTree(MultiSplayTree const &) = delete;
  void operator=(MultiSplayTree const &) = delete;
};

#endif
