/**
 * Assignment 1: priority queue of processes
 * @file pcbtable.h
 * @author Rusty Dillard
 * @brief This is the implementation file for the PCBTable class.
 * //You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
 * // Remember to add sufficient comments to your code
 */

#include "pcbtable.h"

/**
 * @brief Construct a new PCBTable object of the given size (number of PCBs)
 *
 * @param size: the capacity of the PCBTable
 */
PCBTable::PCBTable(int size) {
    // TODO: add your code here
    //  Set the size of the newly minted PCBTable to the size passed into the constructor.
    this->size = size;

    this->count = 0;

    this->table = new PCB * [this->size];

    //  Declare a new array of PCB pointers of length size.
    for (unsigned int i = 0; i < this->size; i++) {
        PCB* newPCB = new PCB(i, i);
        this->table[i] = newPCB;
        this->count++;
    }
}

/**
 * @brief Destroy the PCBTable object. Make sure to delete all the PCBs in the table.
 *
 */
PCBTable::~PCBTable() {
    // TODO: add your code here
    //  Loop through the PCB tabel and delete each pcb in the table.
    for (int i = 0; i < this->size; i++){
        delete this->table[i];
    }

    //  Release the memory allocated for this pcb table.
    delete[] this->table;
}

/**
 * @brief Get the PCB at index "idx" of the PCBTable.
 *
 * @param idx: the index of the PCB to get
 * @return PCB*: pointer to the PCB at index "idx"
 */
PCB* PCBTable::getPCB(unsigned int idx) {
    //  TODO: add your code here
    //  Return the pcb located at this index in the table.
    cout << "Here i am!\n";
    return this->table[idx];
}

/**
 * @brief Add a PCB to the PCBTable.
 *
 * @param pcb: the PCB to add
 */
void PCBTable::addPCB(PCB *pcb, unsigned int idx) {
    // TODO: add your code here
    //  Set this table's memory at this index to the pcb passed into the function.
    if (this->count <= this->size) {
        this->table[idx] = pcb;
        this->count++;
    }

    else {
        cout << "This table is full.\n";
    }
}
