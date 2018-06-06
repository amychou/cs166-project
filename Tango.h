#ifndef Tango_Included
#define Tango_Included

#include <string>

class Tango{
 public:
  Tango();
  ~Tango();

  bool contains(std::size_t key);

  static std::string name(){
    return "Tango Tree";
  }

 private:
  struct Node{
    std::size_t key;
    Node* p;
    Node* left;
    Node* right;
    std::size_t d;
    std::size_t min_d;
    std::size_t max_d;
    bool is_root;
    bool is_red;

    Node(std::size_t k, std::size_t d=0, std::size_t min_d=0, std::size_t max_d=0, bool is_root=0, bool is_red=0){
      this->key = k;
      this->left = nullptr;
      this->right = nullptr;
      this->d = d;
      this->min_d = min_d;
      this->max_d = max_d;
      this->is_root = is_root;
      this->is_red = is_red;
    }
  };

  mutable Node* root;

  static Node* treeFor(std::size_t low, std::size_t high, std::size_t depth, bool is_root);

  bool search(size_t key);
  Node** cut(Node* root, std::size_t depth);
  Node* join(Node* n1, Node* n2);

  Node* concatenate(Node* n);
  Node* join(Node* n1, Node* n2, Node* k);
  Node* split(Node* root, Node* k);
  
  //void insert(Node* n);
  //void insert_recurse(Node* root, Node* n);
  void insert_repair(Node* n);
  void rotate_left(Node* n);
  void rotate_right(Node* n);
  void check(Node* r);

  size_t bheight(Node* m){
    size_t h = 0;
    Node* n = m;
    while (n && !n->is_root){
      if (!n->is_red)
	h++;
      n = n->left;
    }
    return h;
  }

  void mark(Node* n){
    n->is_root = true;
    n->is_red = false;
  }

  Tango(Tango const &) = delete;
  void operator=(Tango const &) = delete;
};

#endif
