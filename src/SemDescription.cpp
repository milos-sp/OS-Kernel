//
// Created by os on 8/13/22.
//

#include "../h/SemDescription.hpp"

int SemDescription::wait() {
    if(isClosed) return -1;
    if(--val < 0){
        blocked.put(PCB::running);
        block();
    }
    return 0;
}

int SemDescription::signal() {
    if(isClosed) return -1;
    if(++val <= 0){
        PCB* thread = blocked.get();
        unblock(thread);
    }
    return 0;
}

int SemDescription::close() { //ukloni sve iz liste blokiranih i val postavi na 0
    if(isClosed) return -1;
    isClosed = true;
    while(blocked.size() > 0){
        PCB* thread = blocked.get();
        unblock(thread);
    }
    val = 0;
    return 0;
}

int SemDescription::block() { //running nit se blokira na semaforu
    if(PCB::running->status == PCB::ThreadStatus::RUNNING){
        PCB::running->status = PCB::ThreadStatus::BLOCKED_SEM; //sada ona ceka
        PCB::dispatch();
        return 0;
    }else return -1;
}

int SemDescription::unblock(PCB *thr) { //odblokira nit sa semafora i vrati je u scheduler
    if(thr->status == PCB::ThreadStatus::BLOCKED_SEM){
        thr->status = PCB::ThreadStatus::READY;
        Scheduler::put(thr);
        return 0;
    }else return -1;
}
