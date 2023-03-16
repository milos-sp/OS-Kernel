//
// Created by os on 5/19/22.
//

#include "../h/Riscv.hpp"
#include "../lib/console.h"
#include "../lib/mem.h"
#include "../test/printing.hpp"

void Riscv::popSppSpie() {
    mc_sstatus(SSTATUS_SPP); //rezim privilegija
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void  Riscv::handleTrap() {
    uint64 scause = r_scause();
    uint64 arguments[5];
    moveArguments(arguments);

    if(scause == 0x0000000000000009UL || scause == 0x0000000000000008UL) { //poziv ecall iz korisnickog rezima - sistemski poziv
       //nije prekid, ecall iz S ili U rezima
        volatile uint64 sepc = r_sepc() + 4;
        volatile uint64 sstatus = r_sstatus();

        switch (arguments[0]) {
            case 0x01:{
                void* p = MemoryAllocator::mem_alloc(arguments[1]);
                w_a0((uint64)p);
                break;
            }
            case 0x02:{
                int p = MemoryAllocator::mem_free((void*)arguments[1]);
                w_a0((uint64)p);
                break;
            }
            case 0x11:{
                thread_t* handle = (thread_t*)arguments[1];
                PCB::Body body = (PCB::Body)arguments[2];
                void* arg = (void*)arguments[3];
                uint64* stack = (uint64*)arguments[4];

                *handle = PCB::createThread(body, arg, stack);
                //*handle = PCB::makeOnlyThread(body, arg, stack); //radi i ovako
                //int p = (*handle)->start(); //pokrece prilikom kreiranja
                w_a0(0); break;
            }
            case 0x12: {
                int p = PCB::exit();
                w_a0(p); break;
            }
            case 0x13:{
              //  PCB::timeSliceCounter = 0;
                PCB::dispatch();
                //sstatus = previousPrivilege(sstatus);
                break;
            }
            case 0x14: {
                thread_t* handle = (thread_t*)arguments[1];
                PCB::Body body = (PCB::Body)arguments[2];
                void* arg = (void*)arguments[3];
                uint64* stack = (uint64*)arguments[4];

                *handle = PCB::makeOnlyThread(body, arg, stack);
                w_a0(0);
                break;
            }
            case 0x15:{
                thread_t handle = (thread_t)arguments[1];
                int p = handle->start();
                w_a0(p);
                break;
            }
            case 0x16:{
                PCB::timeSliceCounter = 0;
                PCB::dispatch();
                int p = PCB::running->getID();
                w_a0(p);
                break;
            }
            case 0x17:{
                thread_t handle = (thread_t)arguments[1];
                /*while(!handle->isFinished()){
                    PCB::dispatch();
                }*/
                if(!handle->isFinished()) handle->setStatus(PCB::ThreadStatus::FINISHED);
                delete handle;
                break;
            }
            case 0x21:{
                sem_t *handle = (sem_t*)arguments[1];
                unsigned init = (unsigned)arguments[2];
                new SemDescription(handle, init);
                w_a0(0);
                break;
            }
            case 0x22:{
                sem_t handle = (sem_t)arguments[1];
                int p = handle->close();
                w_a0(p);
                break;
            }
            case 0x23:{
                sem_t handle = (sem_t)arguments[1];
                int p = handle->wait();
                w_a0(p); break;
            }
            case 0x24:{
                sem_t handle = (sem_t)arguments[1];
                int p = handle->signal();
                w_a0(p); break;
            }
            case 0x41:{
                char c = __getc();
                w_a0(c);
                break;
            }
            case 0x42:{
                char c = (char)arguments[1];
                __putc(c);
                break;
            }
            case 0x50:{
                thread_t handle = (thread_t)arguments[1];
                bool flag = false;
                if(handle->status == PCB::ThreadStatus::RUNNING) flag = true;
                handle->setStatus(PCB::ThreadStatus::FINISHED);
                if(flag) PCB::dispatch();
                w_a0(0);
                break;
            }
            case 0x51:{
                thread_t handle = *(thread_t*)arguments[1];
                if(!handle->isFinished()){
                    if(!handle->sem){
                        new SemDescription(&handle->sem, 0);
                    }
                    handle->sem->wait();
                }
                w_a0(0);
                break;
            }
        }
        __asm__ volatile("sd a0, 80(s0)"); //povratna vrednost, bez ovoga mi pregazi a0

        w_sstatus(sstatus);
        w_sepc(sepc);
    }else if(scause == 0x8000000000000001UL){
        //prekid od tajmera
        mc_sip(SIP_SSIP);
        PCB::timeSliceCounter++;
        if (PCB::timeSliceCounter >= PCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            PCB::timeSliceCounter = 0;
            PCB::dispatch();
          //  sstatus = previousPrivilege(sstatus);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }else if(scause == 0x8000000000000009UL){
        //prekid od konzole
        console_handler();
    }else{
        //neki neocekivani razlog za ulazak

        printString("scause: ");
        printInt(scause);
        printString("\n");
    }
}

uint64 Riscv::previousPrivilege(uint64 sstatus) { //prelazi u privilegovani rezim ako je u pitanju kernelska nit
    if(PCB::running == PCB::kernel){
        //vracanje u S rezim
        return sstatus | SSTATUS_SPP;
    }else{
        //U rezim
        return sstatus & ~(SSTATUS_SPP);
    }
}

void Riscv::prevPrivilege() { //prelazi u privilegovani rezim ako je u pitanju kernelska nit
    if(PCB::running == PCB::kernel){
        //vracanje u S rezim
        ms_sstatus(SSTATUS_SPP);
    }else{
        //U rezim
        mc_sstatus(SSTATUS_SPP);
    }
}

