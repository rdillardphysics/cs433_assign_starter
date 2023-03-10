/**
 * Assignment 1: priority queue of processes
 * @file readyqueue.h
 * @author Rusty Dillard
 * @brief This is the implementation file for the ReadyQueue class.
 */
#include <iostream>
#include "readyqueue.h"

using namespace std;
/**
 * @brief Constructor for the ReadyQueue class.
 */
ReadyQueue::ReadyQueue(){
    //  Initialize the data members
    this->length = 0;
    this->capacity = 200;
    this->table = new PCB* [this->capacity];
}

/**
 * @brief Overloaded constructor for the ReadyQueue class with a prescribed capacity.
 * 
 * @param capacity: set the number of spaces allowed for PCBs in the table.
 */
ReadyQueue::ReadyQueue(int capacity) {
    //  Initialize the data members
    this->length = 0;
    this->capacity = capacity;
    this->table = new PCB* [this->capacity];

    //  Loop through and set all of the pointers in the table to null.
    for (int i = 0; i < this->capacity; i++) {
        this->table[i] = nullptr;
    }
}

/**
 * @brief Copy Constructor: Makes a deep copy of the referenced ReadyQueue object.
 * 
 * @param ReadyQueue&:
 *      Referenced 
 */
ReadyQueue::ReadyQueue(const ReadyQueue& rq) {
    //  Initialize the data members
    length = rq.length;
    capacity = rq.capacity;
    for (int i = 0; i < this->capacity; i++) {
        this->table[i] = nullptr;
    }

    //  Loop through the referenced ReadyQueue's table.
    for (int i = 0; i < length; i++) {
        //  Instantiate a PCB object with the attributes of the referenced PCB and store it in the table.
        table[i] = new PCB(rq.table[i]->getID(), rq.table[i]->getPriority(), rq.table[i]->getState());
    }
}

/**
 * @brief Destructor: provides a method of deletion for the ReadyQueue
 *      object's heap memory allocation.
 */
ReadyQueue::~ReadyQueue() {
    delete[] this->table;
}

/**
 * @brief Overloaded operator=
 *      Provides a shallow copy of the ReadyQueue on the right side of
 *      the operator to the ReadyQueue object on the left.
 * 
 * @param ReadyQueue&: a reference to a ReadyQueue object for copying.
 */
ReadyQueue ReadyQueue::operator=(const ReadyQueue& rq) {
    this->length = rq.length;
    this->capacity = rq.capacity;
    this->table = rq.table;

    //  Return the dereferenced ReadyQueue object on the left side of the operator.
    return *this;
}

/**
 * @brief Add a PCB representing a process into the ready queue.
 *
 * @param pcbPtr: the pointer to the PCB to be added
 */
void ReadyQueue::addPCB(PCB* pcbPtr) {
    if (this->length >= this->capacity) {
        cout << "This table is full.\n";
        return;
    }
    //  Set state to ready, add to ready queue, and sift into the maxheap.
    pcbPtr->setState(ProcState::READY);
    this->table[this->length] = pcbPtr;
    siftUp(this->length);
    this->length++;
}

/**
 * @brief Remove and return the PCB with the highest priority from the queue
 *
 * @return PCB*: the pointer to the PCB with the highest priority
 * @return nullptr: if there's nothing in the PCB pointer array
 */
PCB* ReadyQueue::removePCB() {
    //  Error catch for no PCB* in the table.
    if (this->length <= 0) {
        cout << "There is nothing in this table.\n";
        return nullptr;
    }

    //  Base case where there is only one PCB* in the table.
    if (this->length == 1) {
        this->table[0]->setState(ProcState::RUNNING);
        this->length--;
        return this->table[0];
    }

    //  Set the returnable PCB pointer equal to the first PCB pointer in the table.
    PCB* pcbptr = this->table[0];
    pcbptr->setState(ProcState::RUNNING);
    this->length--;

    //  Bring the last PCB pointer to the top of the heap and sift it down
    this->table[0] = this->table[this->length];
    siftDown(0);

    return pcbptr;
}

/**
 * @brief Returns the number of elements in the queue.
 *
 * @return int: the number of PCBs in the queue
 */
int ReadyQueue::size() {
    return this->length;
}

/**
 * @brief Display the PCBs in the queue that are READY.
 */
void ReadyQueue::displayAll() {
    //  Let the user know the ReadyQueue is being displayed via console.
    cout << "Display processes in ReadyQueue:\n";

    //  Loop through the length of the ReadyQueue table and print as needed.
    for (int i = 0; i < this->length; i++) {
        if (this->table[i]->getState() == ProcState::READY) {
            cout << "      ";
            this->table[i]->display();
        }
    }
}

/**
 * @brief Calculate the index of the parent of the given index
 * @return int: the calculated parent index
 */
int ReadyQueue::parent(int i) {
    return (i - 1) / 2;
}

/**
 * @brief calculate the index of the left child of the given index
 * @return int: the calculated index of the left child
 */
int ReadyQueue::left(int i) {
    return 2 * i + 1;
}

/**
 * @brief calculate the index of the right child of the given index
 * @return int: the calculated index of the right child
 */
int ReadyQueue::right(int i) {
    return 2 * i + 2;
}

/**
 * @brief Compare whether the given index is greater than half the length
 * @return TRUE: i is greater than or equal to half the length
 * @return FALSE: i is less than half the length
 */
bool ReadyQueue::isLeaf(int i) {
    return i >= this->length / 2;
}

/**
 * @brief Bring the given index i higher into the heap
 * @param int i: index to start sorting from.
 */
void ReadyQueue::siftUp(int i) {
    //  Loop until i <= 0 and the parent's priority is greater than the child's priority
    while (i > 0 && this->table[parent(i)]->priority < this->table[i]->priority) {
        swap(this->table, i, parent(i));
        i = parent(i);
    }
}

/**
 * @brief Bring the given index i lower into the heap.
 * @param int i: index to start sorting from.
 */
void ReadyQueue::siftDown(int i) {
    //  Loop until the given index i is considered a leaf in the heap.
    while (!isLeaf(i)) {
        int l = left(i);
        int r = right(i);
        int larger = l;

        if (r < this->length) {
            //  Set larger equal to l if its priority is greater than r and vice versa for r.
            larger = (this->table[l]->priority > this->table[r]->priority) ? l : r;
        }

        //  Loop break condition found
        if (this->table[i]->priority >= this->table[larger]->priority) {
            break;
        }

        swap(this->table, i, larger);
        i = larger;
    }
}

/**
 * @brief Swaps the PCB pointers at indices q and i in the table.
 * @param PCB* table[]: The table in which the PCB pointers will be swapped.
 * @param ints q & i: indices at which the PCB's will be swapped.
 */
void ReadyQueue::swap(PCB* table[], int q, int i) {
    PCB* temp = table[q];
    table[q] = table[i];
    table[i] = temp;
}



