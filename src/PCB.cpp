//
// Created by os on 8/2/22.
//

#include "../h/PCB.hpp"
#include "../h/Riscv.hpp"

PCB* PCB::running = nullptr;
uint64 PCB::timeSliceCounter = 0;
PCB* PCB::kernel = nullptr;
int PCB::ID = 0;

PCB *PCB::createThread(PCB::Body body, void* arg, uint64* stack) {
    PCB* thread = makeOnlyThread(body, arg, stack);
    thread->status = ThreadStatus::READY;
    Scheduler::put(thread);
    return thread;
}

PCB *PCB::makeOnlyThread(Body body, void *arg, uint64 *stack) {
    return new PCB(body, arg, stack);
}

/*void PCB::yield() {
    __asm__ volatile("ecall");
}*/

void PCB::dispatch() {
    PCB* old = running;
    if(old->status == ThreadStatus::RUNNING) { old->status = ThreadStatus::READY; Scheduler::put(old); }
    running = Scheduler::get();
    /*if(running->waiting & !Scheduler::isEmpty()){ //mozda while
        old = running; Scheduler::put(old); running = Scheduler::get();
    }*/
    running->status = ThreadStatus::RUNNING;
    Riscv::prevPrivilege();
    PCB::contextSwitch(&old->context, &running->context);
}

PCB* PCB::kernelThreadMaker() {
    if(!kernel){
        PCB* kernelThread = new PCB(); //default konstruktor potreban
        PCB::kernel = PCB::running = kernelThread;
    }
    return kernel;
}

void PCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setStatus(ThreadStatus::FINISHED); //kada se zavrsi moze signal niti koja ceka preko join?
    if(running->sem != nullptr){
        int i = running->sem->value();
        while(i < 0) {
            running->sem->signal();
            i++;
        }
    }
    thread_dispatch();
    //dispatch();
}

int PCB::start() { //kreiranu nit stavlja u scheduler
    if(status == ThreadStatus::CREATED){
        status = ThreadStatus::READY;
        Scheduler::put(this);
        return 0;
    }else return -1;
}

int PCB::exit() { //zavrsava nit
    if(running->status == ThreadStatus::RUNNING){
        running->status = ThreadStatus::FINISHED;
        dispatch();
        return 0;
    }else return -1;
}

