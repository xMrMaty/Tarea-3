#ifndef List_h
#define List_h

typedef struct List List;
typedef struct Node Node;

List * createList(void);

void * firstList(List * list);

void * nextList(List * list);

void * lastList(List * list);

void * prevList(List * list);

void pushFront(List * list, void * data);

void pushBack(List * list, void * data);

void pushCurrent(List * list, void * data);

void * popFront(List * list);

void * popBack(List * list);

void * popCurrent(List * list);

void clearList(List * list);

int countList(List *list);

int sizeList(List * list);

Node *getFirst(List *list);

Node *getNext(Node *node);

#endif /* List_h */


