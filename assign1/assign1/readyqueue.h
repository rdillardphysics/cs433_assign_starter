/**
 * Assignment 1: priority queue of processes
 * @file readyqueue.h
 * @author Rusty Dillard
 * @brief ReadyQueue is a queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient comments to your code
#pragma once

#include "pcb.h"

/**
 * @brief A queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 */
class ReadyQueue {
private:
    // TODO: add your private member variables here
    // choose a data structure for the ReadyQueue. No STL class is allowed.

/**
 * @brief MaxHeap class rearranges an internal or external array into a Max Heap
 *        data structure. Can be implemented in order to perform a heap sort on an
 *        array of integers.
 */
     int length, capacity;
     PCB** table;

public:
    /**
     * @brief Construct a new ReadyQueue object
     *
     */
    ReadyQueue();

    /*
     * @brief Overloaded cunstructor for a ReadyQueue object with a prescribed capacity
     */
    ReadyQueue(int n);

    // Takes in an external array and turns it into a MaxHeap structure.
    void heapify(PCB* arr_in[], int n);

    // Determines if the index is a leaf.
    bool is_leaf(int);

    void sift_down(int N, int i);

    PCB* extract_max();

    /**
     * @brief Heap Sort algorithm.
     *        Time Complexity: O(nlog(n))
     *
     * @param arr - array to be sorted
     * @param n - size of the array
     */
    void heap_sort(PCB* table[], int n);

    /**
     * @brief UnderFlow exception.
     *      Used as an error for an array with an insufficient number of items.
     */
    class UnderFlow{};

    /**
     * @brief Destructor
     */
    ~ReadyQueue() {}

	// You may add additional member functions, but don't change the definitions of the following four member functions.

    /**
     * @brief Add a PCB representing a process into the ready queue.
     *
     * @param pcbPtr: the pointer to the PCB to be added
     */
	void addPCB(PCB* pcbPtr);

    /**
     * @brief Remove and return the PCB with the highest priority from the queue
     *
     * @return PCB*: the pointer to the PCB with the highest priority
     */
	PCB* removePCB();

    /**
     * @brief Returns the number of elements in the queue.
     *
     * @return int: the number of PCBs in the queue
     */
	int size();

     /**
      * @brief Display the PCBs in the queue.
      */
	void displayAll();

};