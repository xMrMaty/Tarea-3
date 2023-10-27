#ifndef heap_h
#define heap_h

typedef struct Heap Heap;

Heap* createHeap();
void* heap_top(Heap* pq);
void heap_push(Heap* pq, void* data, int priority);
void heap_pop(Heap* pq);


#endif /* HashMap_h */

