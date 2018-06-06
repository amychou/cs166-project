#include "Tango.h"
#include <iostream>

using namespace std;

bool Tango::search(size_t key){
  return false;
}

Tango::Node** Tango::cut(Tango::Node* root, size_t depth){
  return nullptr;
}

Tango::Node* Tango::join(Tango::Node* n1, Tango::Node* n2){
  return nullptr;
}

//Red-Black

Tango::Node* Tango::concatenate(Tango::Node* n1, Tango::Node* n2){
  return nullptr;
}

Tango::Node* Tango::split(Tango::Node* root, Tango::Node* k){
  return nullptr;
}

void Tango::insert(Tango::Node* n){
  insert_recurse(root, n);
  insert_repair(n);

  r = n;
  while (r->p)
    r = r->p;
  this->root = r;
}

void Tango::insert_repair(Tango::Node* n){
  if (!n->p || n->marked){
    n->is_red = 0;
  } else if (!n->p->is_red){
    return;
  } else{
    Tango::Node* g = n->p->p;
    if (g->left == n->p && g->right && g->right->is_red ||
	     g->right == n->p && g->left && g->left->is_red){
      g->is_red = true;
      if (g->left)
	g->left->is_red = false;
      if (g->right)
	g->right->is_red = false;
      insert_repair(g);
    } else{
      if (n == g->left->right){
	rotate_left(n->p);
	n = n->left;
      } else if (n == g->right->left){
	rotate_right(n->p);
	n = n->right;
      }

      if (n == n->p->left)
	rotate_right(n->p->p);
      else
	rotate_left(n->p->p);
      n->p->is_red = false;
      n->p->p->is_red = true;
    }
  }
}

void Tango::insert_recurse(Tango::Node* root, Tango::Node* n){//assumes root not null
  if (n->key < root->key){
    if (root->left && !root->left->marked){
      insert_recurse(root->left, n);
      return;
    } else if (root->left && root->left->marked){
      //TODO
    } else
      root->left = n;
  } else {
    if (root->right && !root->right->marked){
      insert_recurse(root->right, n);
      return;
    } else if (root->right && root->right->marked){
      //TODO
    } else
      root->right = n;
  }
  n->parent = root;
  n->is_red = true;
}

void Tango::rotate_left(Tango::Node* n){
  Node* n_new = n->right;
  n->right = n_new->left;
  n->right->p = n;
  n_new->left = n;
  n_new->p = n->p;
  if (n->p && n->p->left == n)
    n->p->left = n_new;
  else if (n->p && n->p->right == n)
    n->p->right = n_new;
  n->p = n_new;
}

void Tango::rotate_right(Tango::Node* n){
  Node* n_new = n->left;
  n->left = n_new->right;
  n->left->p = n;
  n_new->right = n;
  n_new->p = n->p;
  if (n->p && n->p->right == n)
    n->p->right = n_new;
  else if(n->p && n->p->left == n)
    n->p->left = n_new;
  n->p = n_new;
}

//Constructor and Destructors

Tango::Tango(){
  this->root = new Node(0);
  root->is_root = true;
}

Tango::~Tango(){
  //TODO
}

void Tango::check(Tango::Node* r){
}
