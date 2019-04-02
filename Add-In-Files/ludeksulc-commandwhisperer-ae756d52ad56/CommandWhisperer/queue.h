#ifndef QUEUE_H
#define QUEUE_H
#include <string>

//#define TYPE string
using namespace std;
/* Double Link Struture */
struct DLink {
	string commandName;
	struct DLink* next;
	struct DLink* prev;
};

/* ************************************************************************
   Deque ADT based on Circularly-Doubly-Linked List WITH Sentinel
   ************************************************************************ */
struct cirListDeque {
	int size; /* of links in the queue*/
	struct DLink *Sentinel; /*pointer to the sentinel*/
};

struct cirListDeque* createCirListDeque();

int isEmptyCirListDeque(struct cirListDeque* q);

void addBackCirListDeque(struct cirListDeque* q, string command);
void addFrontCirListDeque(struct cirListDeque* q, string command);

string frontCirListDeque(struct cirListDeque* q);
string backCirListDeque(struct cirListDeque* q);

void removeFrontCirListDeque(struct cirListDeque* q);
void removeBackCirListDeque(struct cirListDeque* q);

void freeCirListDeque(struct cirListDeque* q);

void printCirListDeque(struct cirListDeque* q);

void reverseCirListDeque(struct cirListDeque* q);


//Queue Interface
void enqueue(struct cirListDeque *q, string command);
void dequeue(struct cirListDeque *q);

int queueSize(struct cirListDeque *q);

void newCommand(struct cirListDeque *q, string command);
void checkSequenceSize(struct cirListDeque *q);
string createSequence(struct cirListDeque *q);

#endif