//
// Created by os on 8/13/22.
//

#ifndef PROJECT_BASE_SEMDESCRIPTION_HPP
#define PROJECT_BASE_SEMDESCRIPTION_HPP
//#include "../lib/mem.h"
#include "Queue.hpp"
#include "PCB.hpp"
#include "syscall_c.h"
#include "MemoryAllocator.hpp"

//kao klasa sa predavanja
class SemDescription {
public:
    ~SemDescription(){ close(); }

    void* operator new(size_t size){ return MemoryAllocator::mem_alloc(size); }

    void operator delete(void* p){ MemoryAllocator::mem_free(p); }

private:
    int val; //ovde je bio problem sto sam stavio unsigned!
    Queue blocked;
    friend class Riscv;
    friend class PCB;
    bool isClosed;
   // typedef SemDescription* sem_t;
    SemDescription(sem_t* handle, unsigned val) : val(val){
        *handle = this; isClosed = false;
    }
    int value() const{ return val; }
    int wait();
    int signal();
    int close();
    int block();
    int unblock(PCB* thr);
};


#endif //PROJECT_BASE_SEMDESCRIPTION_HPP
