/**
 * Assignment 1: priority queue of processes
 * @file pcbtable.h
 * @author Rusty Dillard
 * @brief This is the implementation file for the PCBTable class.
 */

#include "pcbtable.h"

/**
 * @brief Overloaded constructor for the PCBTable class.
 *
 * @param size: the capacity of the PCBTable
 */
PCBTable::PCBTable(int size) {
    //  Initialize the data members
    this->size = size;
    this->count = 0;
    this->table = new PCB * [this->size];

    //  Loop through and set all of the pointers in the table to null.
    for (int i = 0; i < this->size; i++) {
        this->table[i] = nullptr;
    }
}

/**
 * @brief Base constructor for the PCBTable class.
 */
PCBTable::PCBTable() {
    //  Initialize the data members.
    this->size = 100;
    this->count = 0;
    this->table = new PCB * [this->size];
    
    //  Loop through and set all of the pointers in the table to null.
    for (int i = 0; i < this->size; i++) {
        this->table[i] = nullptr;
    }
}

/**
 * @brief Destructor: releases all of the heap memory allocation associated with the PCBTable.
 */
PCBTable::~PCBTable() {
    for (int i = 0; i <= this->count; i++){
        delete this->table[i];
    }
    delete[] this->table;
}

/**
 * @brief Get the PCB at index "idx" of the PCBTable.
 *
 * @param idx: the index of the PCB to get.
 * @return PCB*: pointer to the PCB at index "idx".
 */
PCB* PCBTable::getPCB(unsigned int idx) {
    return this->table[idx];
}

/**
 * @brief Add a PCB pointer to the PCBTable.
 *
 * @param pcb: the PCB pointer to add
 * @param idx: index at which to store the PCB pointer
 */
void PCBTable::addPCB(PCB *pcb, unsigned int idx) {
    if (this->count < this->size) {
        this->table[idx] = pcb;
        this->count++;
    }

    else {
        cout << "This table is full.\n";
    }
}
