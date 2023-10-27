#include "heap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo {
  void *data;
  int priority;
} heapElem;

typedef struct Heap {
  heapElem *heapArray;
  int size;
  int capac;
} Heap;

void *heap_top(Heap *pq) {
  if (pq == NULL || pq->size == 0)
    return NULL;

  return pq->heapArray[0].data;
}

void heap_push(Heap *pq, void *data, int priority) {
  if (pq == NULL || pq->size == 0)
    return;

  if (pq->size == pq->capac) // si esta lleno
  {
    int nueva_capacidad = (2 * pq->capac) + 1;
    pq->heapArray = realloc(pq->heapArray, nueva_capacidad * sizeof(heapElem));
    pq->capac = nueva_capacidad;
  }

  int i = pq->size; // almacenar en espacio disponible
  pq->heapArray[i].data = data;
  pq->heapArray[i].priority = priority;
  // organizar array
  while (i > 0 &&
         pq->heapArray[i].priority > pq->heapArray[(i - 1) / 2].priority)
  // Mientras que el indice no sea la raiz y que la prioridad del nuevo dato no
  // sea mayor que la del padre
  {
    heapElem temp = pq->heapArray[i];
    pq->heapArray[i] = pq->heapArray[(i - 1) / 2];
    pq->heapArray[(i - 1) / 2] = temp;
    i = (i - 1) / 2;
  }
  pq->size++;
}

void heap_pop(Heap *pq) {
  if (pq == NULL || pq->size == 0)
    return;
  
  pq->size--; // reducir tamanyo
  pq->heapArray[0] = pq->heapArray[pq->size];

  int padre =0;

  while((2*padre)+1 < pq->size ) // buscar padres
  {
    int hijo = 2*padre +1;
    if(hijo+1 < pq->size && pq->heapArray[hijo + 1].priority > pq->heapArray[hijo].priority) // hijo de mayor prioridad
    {
      hijo++;
    }

     if (pq->heapArray[padre].priority < pq->heapArray[hijo].priority) // si el hijo tiene mayor prioridad que el padre
     {
       heapElem temp = pq->heapArray[padre];
       pq->heapArray[padre] = pq->heapArray[hijo];
       pq->heapArray[hijo] = temp;
       padre = hijo;
     }
      else // si no, ya esta ordenado
     {
       break;
     }
  }
}

Heap *createHeap() {
  Heap *dato = malloc(sizeof(Heap));
  dato->capac = 3;
  dato->size = 0;
  dato->heapArray = malloc(3 * sizeof(heapElem));
  return dato;
}


// para encontrar nodos padre = 2 * i + 1, (doble + 1)