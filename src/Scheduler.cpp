//
// Created by os on 8/2/22.
//

#include "../h/Scheduler.hpp"
#include "../h/PCB.hpp"

List<PCB> Scheduler::readyQueue;
int Scheduler::cnt = 0;

PCB *Scheduler::get(){
    cnt--;
    return readyQueue.removeFront();
}

void Scheduler::put(PCB *pcb) {
    cnt++;
    readyQueue.addEnd(pcb);
}

bool Scheduler::isEmpty() {
    return cnt == 0;
}

