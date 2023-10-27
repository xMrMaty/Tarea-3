#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

/*int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}*/

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap* mapa = (TreeMap*)malloc(sizeof(TreeMap));
  mapa->root = NULL;
  mapa->current = NULL;
  mapa->lower_than = lower_than;
  return mapa;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if(searchTreeMap(tree, key) != NULL){
    return;
  }
  TreeNode* nuevo = createTreeNode(key,value);
  if(tree->root == NULL){
    tree->current = nuevo;
    tree->root= nuevo;
    return;
  }
  TreeNode* current = tree->root;
  while(1){
    if(tree->lower_than(nuevo->pair->key,current->pair->key)){
      if(current->left == NULL){
        current->left = nuevo;
        nuevo->parent = current;
        tree->current = nuevo;
        return;
      }
      current = current->left;
    }else{
      if(current->right == NULL){
        current->right = nuevo;
        nuevo->parent = current;
        tree->current = nuevo;
        return;
      }
      current = current->right;
    }
  }
}

TreeNode * minimum(TreeNode * x){
  while(x->left != NULL){
    x = x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if(node->right == NULL && node->left == NULL){
    if(node->parent->left == node){
      node->parent->left = NULL;
    }else{
      node->parent->right = NULL;
    }
  }else if(node->right == NULL || node->left == NULL){
    TreeNode* a = NULL;
    if(node->left != NULL){
      a = node->left;
    }else{
      a = node->right;
    }
    if(node->parent->left == node){
      node->parent->left = a;
    }else{
      node->parent->right = a;
    }
    a->parent = node->parent;
  }else{
    TreeNode* b = minimum(node->right);
    node->pair->key = b->pair->key;
    node->pair->value= b->pair->value;
    removeNode(tree,b);
  }
  
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode* current = tree->root;
  while(current != NULL){
    if(is_equal(tree,key,current->pair->key)){
      tree->current = current;  
      return current->pair;
    }else if(tree->lower_than(key, current->pair->key)){
      current= current->left;
    }else{
      current= current->right;
      
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode* current= tree->root;
  TreeNode* nodo = NULL;
  while(current!=NULL){
    if(tree->lower_than(key,current->pair->key)){
      nodo = current;
      current= current->left;
      
    }else if(is_equal(tree,key,current->pair->key)){
      tree->current= current;
      return current->pair;
    }else{
      current= current->right;
    }
  }
  if(nodo!=NULL){
    tree->current=nodo;
    return nodo->pair;
  }
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode* current= tree->root;
  while(current!=NULL && current->left!=NULL){
    current=current->left;
    
  }
  if(current!=NULL){
    tree->current= current;
    return current->pair;
  }
  return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
  TreeNode* current= tree->current;
  if(current->right != NULL){
    current= current->right;
    while(current->left != NULL){
      current= current->left;
    }
    tree->current=current;
    return current->pair;
  }
  while(current->parent!=NULL && current->parent->right==current){
    current=current->parent;
  }
  if(current->parent!=NULL){
    current= current->parent;
    tree->current= current;
    return current->pair;
  }
  return NULL;
}

Pair *lastTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode *current = tree->root;
    while (current->right != NULL) {
        current = current->right;
    }

    tree->current = current;
    return current->pair;
}

