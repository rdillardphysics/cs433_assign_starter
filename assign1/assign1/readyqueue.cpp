#include <iostream>
#include "readyqueue.h"

using namespace std;

//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient comments to your code

void swap(PCB* arr[], int q, int i)
{
    PCB* temp = arr[q];
    arr[q] = arr[i];
    arr[i] = temp;
}



/**
 * @brief Constructor for the ReadyQueue class.
 */
ReadyQueue::ReadyQueue(){
    //TODO: add your code here
    this->length = 0;
    this->capacity = 100;
    this->table = new PCB* [this->capacity];
}

ReadyQueue::ReadyQueue(int n) {
    this->length = 0;
    this->capacity = n;
    this->table = new PCB* [this->capacity];
}

PCB* ReadyQueue::extract_max() {
    if (this->length <= 0) {
        throw UnderFlow();
    }

    if (this->length == 1) {
        return this->table[--this->length];
    }

    PCB* res = this->table[0];
    this->table[0] = this->table[--this->length];
    this->sift_down(this->length, 0);

    return res;
}

void ReadyQueue::sift_down(int N, int i) {
    while (!this->is_leaf(i)) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int larger = left;
        if (left < N && this->table[left]->getPriority() > this->table[larger]->getPriority()) {
            larger = left;
        }

        if (right < N && this->table[right]->getPriority() > this->table[larger]->getPriority()) {
            larger = right;
        }

        if (larger != i) {
            swap(this->table, i, larger);

            heapify(this->table, larger);
        }
    }
}

void ReadyQueue::heapify(PCB* arr_in[], int n) {
    this->table = arr_in;
    this->length = n;
    this->capacity = n;
    int half = this->length / 2 - 1;

    for (int i = half; i >= 0; i--) {
        this->sift_down(this->length, i);
    }
}

void ReadyQueue::heap_sort(PCB* arr_in[], int n) {
    ReadyQueue rq(n);
    rq.heapify(arr_in, n);
    int i = n - 1;

    while (rq.size()) {
        this->table[--i] = rq.extract_max();
    }
}

bool ReadyQueue::is_leaf(int i) {
    return (i >= this->length / 2);
}

/**
 * @brief Add a PCB representing a process into the ready queue.
 *
 * @param pcbPtr: the pointer to the PCB to be added
 */
void ReadyQueue::addPCB(PCB* pcbPtr) {
    //TODO: add your code here
    // When adding a PCB to the queue, you must change its state to READY.
    this->length++;
    pcbPtr->setState(ProcState::READY);
    this->table[this->length-1] = pcbPtr;
    heapify(this->table, this->length);
    heap_sort(this->table, this->length);
}

/**
 * @brief Remove and return the PCB with the highest priority from the queue
 *
 * @return PCB*: the pointer to the PCB with the highest priority
 */
PCB* ReadyQueue::removePCB() {
    // TODO: add your code here
    // When removing a PCB from the queue, you must change its state to RUNNING.
    PCB* temp = extract_max(); 
    temp->setState(ProcState::RUNNING);
    return temp;
}

/**
 * @brief Returns the number of elements in the queue.
 *
 * @return int: the number of PCBs in the queue
 */
int ReadyQueue::size() {
    //TODO: add your code here
    return this->length;
}

/**
 * @brief Display the PCBs in the queue.
 */
void ReadyQueue::displayAll() {
    //TODO: add your code here
    cout << "Display processes in ReadyQueue:\n";
    for (int i = 0; i < this->length; i++) {
        cout << "/t";
        this->table[i]->display();
    }
}