#include "queue.h"
#include <iostream>
#include <string>
#include <assert.h>   
#include "StdAfx.h"
///#define TYPE string
#define SEQ_SIZE 5

using namespace std;
/* ************************************************************************
	Deque ADT based on Circularly-Doubly-Linked List WITH Sentinel
	************************************************************************ */

	/* internal functions interface */
struct DLink* _createLink(string commandName);
void _addLinkAfter(struct cirListDeque* q, struct DLink* lnk, string newCommand);
void _removeLink(struct cirListDeque* q, struct DLink* lnk);


/* ************************************************************************
   Deque Functions
   ************************************************************************ */

   /*Initialize deque

	   param: qpointer to the deque
	   pre: q is not null
	   post: q->Sentinel is allocated and q->size equals zero
   */
void _initCirListDeque(struct cirListDeque *q) {
	q->Sentinel = _createLink("Sentinel");
	q->Sentinel->prev = q->Sentinel;
	q->Sentinel->next = q->Sentinel;
	q->size = 0;
}

/*Create a new circular list deque*/
struct cirListDeque *createCirListDeque() {
	cirListDeque *newCL = new cirListDeque();
	_initCirListDeque(newCL);
	return(newCL);
}

/*Create a link for a command name
	param: string name create a new link for new command
	pre: none
	post: a link to store the command name
*/
struct DLink *_createLink(string command) {
	/*allocate memory for the new link*/
	DLink* lnk = new DLink();
	assert(lnk != 0);

	lnk->commandName = command;
	return lnk;
}

/* Adds a link after another link
	param: qpointer to the deque
	param: lnkpointer to the existing link in the deque
	param: command name of the new link to be added after the current link
	pre: q is not null
	pre: lnk is not nul
	pre: lnk is in the deque
	post: the new link is added into the deque after the existing link
*/
void _addLinkAfter(struct cirListDeque *q, struct DLink *lnk, string newCommand) {
	/* create the new link */
	struct DLink *newLnk = _createLink(newCommand);

	/* re-allocate pointers */
	newLnk->next = lnk->next;
	newLnk->prev = lnk;
	lnk->next->prev = newLnk;
	lnk->next = newLnk;

	/* increase size of the deque */
	++(q->size);
}

/* Adds a link to the back of the deque

	param: qpointer to the deque
	param: string name for the link to be added
	pre: q is not null
	post: a link storing command name is added to the back of the deque
*/
void addBackCirListDeque(struct cirListDeque *q, string command) {
	/*add the link before the Sentinel */
	_addLinkAfter(q, q->Sentinel->prev, command);
}

/* Adds a link to the front of the deque

	param: qpointer to the deque
	param: command for the link to be added
	pre: q is not null
	post: a link storing the command is added to the front of the deque
*/

void addFrontCirListDeque(struct cirListDeque *q, string command) {
	/* add the link after the Sentinel */
	_addLinkAfter(q, q->Sentinel, command);
}

/*Get the command from the front of the deque
	param: qpointer to the deque
	pre: q is not null and q is not empty
	post: none
	return: comand from front of the deque
*/

string frontCirListDeque(struct cirListDeque *q) {
	assert(!isEmptyCirListDeque(q));
	return q->Sentinel->next->commandName;
}

/* Get the command from the back of the deque
	param: qpointer to the deque
	pre: q is not null and q is not empty
	post: none
	ret: command from the back of the deque
*/
string backCirListDeque(struct cirListDeque *q) {
	assert(!isEmptyCirListDeque(q));
	return q->Sentinel->prev->commandName;
}

/* Remove a link from the deque
	param: qpointer to the deque
	param: lnkpointer to the link to be removed
	pre: q is not null and q is not empty
	pre: lnk is in the deque
	post: the link is removed from the deque
*/

void _removeLink(struct cirListDeque *q, struct DLink *lnk) {
	assert(!isEmptyCirListDeque(q));
	lnk->next->prev = lnk->prev;
	lnk->prev->next = lnk->next;
	delete lnk;
	--(q->size);
}

/*Remove the front of the deque
	param: qpointer to the deque
	pre: q is not null and q is not empty
	post: the front is removed from the deque
*/
void removeFrontCirListDeque(struct cirListDeque *q) {
	_removeLink(q, q->Sentinel->next);
}

/* Remove the back of the deque
	param: qpointer to the deque
	pre: q is not null and q is not empty
	post: the back is removed from the deque
*/
void removeBackCirListDeque(struct cirListDeque *q) {
	_removeLink(q, q->Sentinel->prev);
}

/* De-allocate all links of the deque
	param: qpointer to the deque
	pre: none
	post: All links (including the Sentinel) are de-allocated
*/
void freeCirListDeque(struct cirListDeque *q) {
	while (!isEmptyCirListDeque(q))
		removeFrontCirListDeque(q);
	delete q->Sentinel;
}

/* Check wheter the deque is empty
	param: qpointer to the deque
	pre: q is not null
	return: 1 if the deque is empty. Otherwise, 0.
*/
int isEmptyCirListDeque(struct cirListDeque *q) {
	return (q->size == 0);
}

/* Print the links in the deque from front to back debug purposes
	param: qpointer to the deque
	pre: q is not null and q is not empty
	post: the links in the deque are printed from front to back
*/
void printCirListDeque(struct cirListDeque *q) {
	struct DLink *lnk;
	assert(!isEmptyCirListDeque(q));
	lnk = q->Sentinel->next;
	do {
		cout << lnk->commandName << endl;
		lnk = lnk->next;
	} while (lnk != q->Sentinel);

	cout << "******************" << endl;
}

/***************************************************************
 * Queue Interface
 * The sequence of commands is saved in this queue
 * The size of the sequence is set def SEQ_SIZE (int value)
 * Once the queue size == SEQ_SIZE it begins a routine
 * The routine:
		* The queue concatenates all of the command names from the links
		  to be piped to ML alg.
		* Removes oldest link(command) in the queue.
		* Only allows queue size to reach SEQ_SIZE
 ***************************************************************/

 /* Adds new command in sequence
	 param: qpointer to deque
	 param: new cammand
	 pre: q is not null
	 post: a link storing command name is added to the back of the deque
 */
void enqueue(struct cirListDeque *q, string command) {
	addBackCirListDeque(q, command);
}

/* Removes oldest command in sequence
	param: qpointer to deque
	pre: q is not null
	post: the link storing the oldest command in sequence is removed from deque
*/
void dequeue(struct cirListDeque *q) {
	removeFrontCirListDeque(q);
}

/* Handles new comands as they are entered by user
	*Important note: This function is called before a sequence is created because
					 it handles when a new sequence is created.
	param: qpointer to deque
	param: new command as a string
	pre: q is or is not null
	post: Adds new
*/
void newCommand(struct cirListDeque *q, string command) {
	/* first add the new command to queue */
	enqueue(q, command);
	/* Check size of queue */
	if (queueSize(q) == 6)
		dequeue(q);
}

/* Keeps track of the size of the queue
	param: qpointer to deque
	pre: q is not null
	return: Returns size of queue
*/
int queueSize(struct cirListDeque *q) {
	return q->size;
}

/* Concatenates the command names with delimiters for piping the seqeunce to ML
	param: qpointer to deque
	pre: q is not null and queue size == SEQ_SIZE
	return: returns concatenated sequence of commands
*/
string createSequence(struct cirListDeque *q) {
	struct DLink *lnk;
	string sequence; /*Beginning sequence will be null*/
	assert(!isEmptyCirListDeque(q));
	lnk = q->Sentinel->next;
	do {
		sequence = lnk->commandName + '/' + sequence;
		lnk = lnk->next;
	} while (lnk != q->Sentinel);

	return sequence;
}