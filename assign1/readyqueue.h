/**
 * Assignment 1: priority queue of processes
 * @file readyqueue.h
 * @author Rusty Dillard
 * @brief ReadyQueue is a queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 * @version 0.1
 */
#pragma once

#include "pcb.h"

/**
 * @brief A queue of PCB's that are in the READY state to be scheduled to run.
 * It should be a priority queue such that the process with the highest priority can be selected next.
 */
class ReadyQueue {
    //  Members only accessible from within the class.
    private:
        int length, capacity;
        PCB** table;

    //  Members accessible from anywhere the readyqueue.h file is included.
    public:
        /**
         * @brief Construct a new ReadyQueue object.
         *
         */
        ReadyQueue();

        /**
         * @brief Overloaded constructor for a ReadyQueue object with a prescribed capacity.
         */
        ReadyQueue(int capacity);

        /**
         * @brief Copy constructor: makes a deep copy of the referenced ReadyQueue object.
         */
        ReadyQueue(const ReadyQueue& rq);

        /**
         * @brief Destructor
         */
        ~ReadyQueue();

        /**
         * @brief Overloaded operator= for ReadyQueue objects on both sides.
         */
        ReadyQueue operator=(const ReadyQueue& rq);
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

        /**
         * @brief Bring the given index i higher into the heap
         */
        void siftUp(int i);

        /**
         * @brief move the given index i lower into the heap
         */
        void siftDown(int i);

        /**
         * @brief Calculate the parent index of i
         */
        int parent(int i);

        /**
         * @brief Calculate the left child index of i
         */
        int left(int i);

        /**
         * @brief Calculate the right child index of i
         */
        int right(int i);

        /**
         * @brief Determine if the given index is a leaf
         */
        bool isLeaf(int i);

        /**
         * @brief swap PCB pointers at indices q and i in the PCB pointer table.
         */
        void swap(PCB* table[], int q, int i);
};