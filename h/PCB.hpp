//
// Created by os on 8/2/22.
//

#ifndef PROJECT_BASE_PCB_HPP
#define PROJECT_BASE_PCB_HPP


#include "../lib/hw.h"
#include "Scheduler.hpp"
#include "syscall_c.h"

class PCB{

public:
    bool waiting = false;

    sem_t sem = nullptr;

    ~PCB(){ delete []stack; }

    using Body = void(*)(void*);

    static PCB* createThread(Body body, void* arg, uint64* stack); //odmah zapocinje

    static PCB* makeOnlyThread(Body body, void* arg, uint64* stack); //napravi pa se posle moze pokrenuti

    static PCB* kernelThreadMaker();

    void* operator new(size_t size){ return MemoryAllocator::mem_alloc(size); }

    void operator delete(void* p){ MemoryAllocator::mem_free(p); }

    //static void yield();

    enum ThreadStatus{
        RUNNING, READY,
        FINISHED, CREATED,
        BLOCKED_SEM
    };

    bool isFinished() const{
        return status == ThreadStatus::FINISHED;
    }

    void setStatus(ThreadStatus ts){
        status = ts;
    }

    uint64 getTimeSlice() const { return timeSlice; }

    static PCB* running;


    static PCB* kernel; //nit koja se izvrsava u privilegovanom rezimu

    int start();

    static int exit();

    static int ID;

    int getID() const { return id; }
    static uint64 timeSliceCounter;
private:
    struct Context{
        uint64 ra;
        uint64 sp;
    };
    Body body;
    uint64* stack;
    void* arg;
    Context context;
   // bool finished;
    ThreadStatus status;
    uint64 timeSlice;
    int id;



    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
    static void dispatch();
    static void contextSwitch(Context* oldC, Context* newC);
    static void threadWrapper();

    explicit PCB(Body body, void* arg, uint64* stack) : body(body), stack(stack), arg(arg),
    context({(uint64)&threadWrapper,(uint64)&stack[DEFAULT_STACK_SIZE]}), status(ThreadStatus::CREATED), timeSlice(TIME_SLICE), id(ID++)
    {
      //  if(body != nullptr) Scheduler::put(this);
    }
    PCB() : context({0, 0}) {
        body = nullptr; arg = nullptr; stack = nullptr;
        timeSlice = DEFAULT_TIME_SLICE; status = ThreadStatus::RUNNING;
    }
    friend class Thread;
    friend class Riscv;
    friend class SemDescription;

};


#endif //PROJECT_BASE_PCB_HPP
