#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;
int enlarge_called=0;


struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map, char * key, void * value) {
    long idx = hash(key, map->capacity);
    
    while (map->buckets[idx] != NULL && map->buckets[idx]->key != NULL){
        if(is_equal(map->buckets[idx]->key, key) == 1) return; //el dato ya existe
        idx = (idx + 1) % map->capacity;
    }
    
    if (map->buckets[idx] != NULL) {
        map->buckets[idx]->key = key;
        map->buckets[idx]->value = value;
    } else 
        map->buckets[idx] = createPair(key, value);
    
    
    map->size += 1;
    
    if ((double)map->size/(double)map->capacity > 0.75) enlarge(map);
}

void enlarge(HashMap * map) {
    map->capacity *= 2;

    Pair ** oldBucket = map->buckets;
    
    long oldCapacity = map->capacity/2;

    map->buckets = (Pair **)malloc(sizeof(Pair *) * map->capacity);
    memset(map->buckets, 0, map->capacity * sizeof(Pair *));
    map->size = 0;
    long i;
    
    for (i = 0; i < oldCapacity; i++) {
        if (oldBucket[i] != NULL) {
            if (oldBucket[i]->value != NULL) {
                insertMap(map, oldBucket[i]->key, oldBucket[i]->value);
            } else {
                free(oldBucket[i]);
            }
        }
    }
    
    
    free(oldBucket);
}


HashMap * createMap(long capacity) {
    HashMap * new = (HashMap *)malloc(sizeof(HashMap));
    new->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
    new->size = 0;
    new->capacity = capacity;
    new->current = -1;
    return new;
}

void eraseMap(HashMap * map,  char * key) {    
    long idx = hash(key, map->capacity);
    while (map->buckets[idx] != NULL && is_equal(map->buckets[idx]->key, key) == 0)
        idx = (idx + 1) % map->capacity;
    
    if (map->buckets[idx] == NULL) return;

    map->buckets[idx]->key = NULL;

    map->size--;

}

Pair * searchMap(HashMap * map,  char * key) {   
    long idx = hash(key, map->capacity);
    while (map->buckets[idx] != NULL && is_equal(map->buckets[idx]->key, key) == 0) 
        idx = (idx + 1) % map->capacity;
    
    if (map->buckets[idx] == NULL || map->buckets[idx]->value == NULL) return NULL;
    
    map->current = idx;
    
    return (void *)map->buckets[idx];
}

Pair * firstMap(HashMap * map) {
    if (map == NULL || map->buckets == NULL) return NULL;
    
    long i;
    
    for (i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map == NULL || map->buckets == NULL) return NULL;
    
    long i;
    
    for (i = (map->current + 1); i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    
    return NULL;
}

void destroyHashMap(HashMap *map) {
  if (map == NULL) {
    return;
  }

  Pair *currentPair = firstMap(map);
  while (currentPair != NULL) {
    Pair *nextPair = nextMap(map);
    
    free(currentPair->value);

    free(currentPair->key);

    free(currentPair);

    currentPair = nextPair;
  }
  free(map);
}

int size(HashMap *map) {
    if (map == NULL) {
        return 0;
    }

    int count = 0;
    for (long i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            count++;
        }
    }

    return count;
}
