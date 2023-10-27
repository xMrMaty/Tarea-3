#ifndef TREEMAP_h
#define TREEMAP_h
#include "hashmap.h"
typedef struct TreeMap TreeMap;
/*
typedef struct Pair {
     void * key;
     void * value;
} Pair;
*/
TreeMap * createTreeMap(int (*lower_than_int) (void* key1, void* key2));

void insertTreeMap(TreeMap * tree, void* key, void * value);

void eraseTreeMap(TreeMap * tree, void* key);

Pair * searchTreeMap(TreeMap * tree, void* key);

Pair * upperBound(TreeMap * tree, void* key);

Pair * firstTreeMap(TreeMap * tree);

Pair * nextTreeMap(TreeMap * tree);

Pair *lastTreeMap(TreeMap * tree);

int sizeTreeMap(TreeMap * tree);

#endif /* TREEMAP_h */
