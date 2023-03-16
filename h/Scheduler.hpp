//
// Created by os on 8/2/22.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP
#include "list.hpp"
#include "MemoryAllocator.hpp"

class PCB;

class Scheduler{
private:
    static List<PCB> readyQueue;
    static int cnt;

public:
    static PCB* get();

    static void put(PCB* pcb);

    static bool isEmpty();
};

#endif //PROJECT_BASE_SCHEDULER_HPP
