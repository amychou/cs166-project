#include "Tango.h"
#include <iostream>
#include <stack>

using namespace std;

bool Tango::search(size_t key){
  return false;
}

Tango::Node** Tango::cut(Tango::Node* root, size_t depth){
  bool to_mark = root->is_root;
  root->is_root = false;
  Tango::Node* l = root->max_d > d ? root : nullptr;
  while (l && l->left && !l->left->is_root && l->left->max_d > d)
    l = l->left;
  Tango::Node* r = root->max_d > d ? root : nullptr;
  while (r && r->right && !r->right->is_root && r->right->max_d > d)
    r = r->right;

  Tango::Node* lt = root;
  Tango::Node* rt = root;
  if (l){
    lt = split(root, l);
    rt = lt->right;
  }
  if (r){
    rt = split(rt, r);
  }

  droot = r ? rt->left : (l ? lt->right : root);
  if (droot)
    mark(droot);

  Tango::Node* f = root;
  if (r)
    f = concatenate(r);
  if (l)
    f = concatenate(l);
  f->is_root = to_mark;
  return f;
}

Tango::Node* Tango::join(Tango::Node* n1, Tango::Node* n2){
  if (n1->max_d < n2->max_d){
    Tango::Node* temp = n1;
    n1 = n2;
    n2 = temp;
  }
  Tango::Node* l = nullptr;
  Tango::Node* r = nullptr;
}

//Red-Black

Tango::Node* Tango::concatenate(Tango::Node* n){
  return join(n->left, n->right, n);
}

Tango::Node* Tango::join(Tango::Node* n1, Tango::Node* n2, Tango::Node* k){
  h1 = bheight(n1);
  h2 = bheight(n2);
  Tango::Node* gp = k->p;
  if (h1 == h2){
    return;
    /*k->is_root = r;
    k->is_red = false;
    k->left = n1;
    n1->p = k;
    k->right = n2;
    n2->p = k;
    n1->is_root = false;
    n2->is_root = false;*/
  } else if (h1 > h2){
    Tango::Node* c = n1;
    n1->p = gp;
    n1->is_root = k->is_root;
    k->is_root = false;
    while (h1 > h2 || c->is_red){
      if (!c->is_red)
	--h1;
      c = c->right;
    }
    k->is_red = true;
    c->p->right = k;
    k->p = c->p;
    k->left = c;
    c->p = k;
    k->right = n2;
    n2->p = k;
    insert_repair(k);
  } else{
    Tango::Node* c = n2;
    n2->p = gp;
    n2->is_root = k->is_root;
    k->is_root = false;
    while (h2 > h1 || c->is_red){
      if (!c->is_red)
	--h2;
      c = c->left;
    }
    k->is_red = true;
    c->p->left = k;
    k->p = c->p;
    k->right = c;
    c->p = k;
    k->left = n1;
    n1->p = k;
    insert_repair(k);
  }
  while (k->p != gp){
    k = k->p;
  }
  return k;
}

Tango::Node* Tango::split(Tango::Node* root, Tango::Node* k){
  stack<Tango::Node*> lp;
  stack<Tango::Node*> rp;
  Tango::Node* curr = root;
  while (curr != k){
    if (curr->key > k->key){
      rp.push(curr);
      curr = curr->left;
    } else{
      lp.push(curr);
      curr = curr->right;
    }
  }
  
  Tango::Node* l = nullptr;
  if (!lp.empty()){
    l = lp.pop();
    l->is_red = false;
  }
  while (!lp.empty()){
    Tango::Node* n = lp.pop();
    n2 = n->left;
    if (n2){
      n2->is_red = false;
    }
    l = join(n2, l, n);
  }

  if (!rp.empty()){
    r = rp.pop();
    r->is_red = false;
  }
  while (!rp.empty()){
    Tango::Node* n = rp.pop();
    n2 = n->right;
    if (n2){
      n2->is_red = false;
    }
    r = join(r, n2, n);
  }
  if (r)
    r->p = k;
  if (l)
    l->p = k;
  k->is_red = false;
  k->left = l;
  k->right = r;
  
  return k;
}

/*void Tango::insert(Tango::Node* n){
  n->is_root = false;
  insert_recurse(root, n);
  insert_repair(n);

  r = n;
  while (!r->is_root)
    r = r->p;
  this->root = r;
  r->is_root = true;
  }*/

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

/*void Tango::insert_recurse(Tango::Node* root, Tango::Node* n){//assumes root not null
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
  }*/

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
  else if (n->is_root){
    n->is_root = false;
    n_new->is_root = true;
  }
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
  else if (n->is_root){
    n->is_root = false;
    n_new->is_root = true;
  }
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
