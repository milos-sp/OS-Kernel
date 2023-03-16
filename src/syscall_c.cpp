//
// Created by os on 5/18/22.
//

#include "../h/syscall_c.h"

//pozivi
void* mem_alloc(size_t size){
    uint64 code = 0x01;
    void* p;
    if(size <= 0) return nullptr;
    __asm__ volatile("mv a1, %0" : : "r" (size));
    __asm__ volatile("mv a0, %0" : : "r" (code)); //nesto ovako? ali u a0 ide onaj kod 0x01
    __asm__ volatile("ecall"); //bp
    __asm__ volatile("mv %0, a0" : "=r"(p)); //bp

    return p;
    //u stvec ide nesto verovatno pa ecall
}

int mem_free(void* adr){
    uint64 code = 0x02;
    int p;
    __asm__ volatile("mv a1, %0" : : "r" (adr));
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return p;
}

int thread_help(uint64 code, thread_t* handle, void(*start_routine)(void*), void* arg, void* stack){//bez ove funkcije ne prenosi ispravno argumente
    __asm__ volatile("ecall");
    uint64 volatile p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return p;
}
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){

    if(!handle) return -1;
    void* stack = mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));
    if(stack == nullptr) return -2;
    return thread_help(0x11, handle, start_routine, arg, stack);
}

int thread_exit(){
    uint64 code = 0x12;
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    int p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return p;
}

void thread_dispatch(){
    uint64 code = 0x13;
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
}

int sem_open(sem_t* handle, unsigned init){
    uint64 code = 0x21;
    if(!handle) return -1;
    __asm__ volatile("mv a2, a1");
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    int p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return p;
}

int sem_close(sem_t handle){
    uint64 code = 0x22;
    if(!handle) return -1;
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    int p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return p;
}

int sem_wait(sem_t id){
    uint64 code = 0x23;
    if(!id) return -1;
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    int p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return p;
}

int sem_signal(sem_t id){
    uint64 code = 0x24;
    if(!id) return -1;
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    int p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return p;
}

int thread_create_without_start(thread_t* handle, void(*start_routine)(void*), void* arg){
    if(!handle) return -1;
    void* stack = mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));
    if(stack == nullptr) return -2;
    return thread_help(0x14, handle, start_routine, arg, stack);
}
int thread_start(thread_t handle){
    uint64 code = 0x15;
    if(!handle) return -1;
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    uint64 volatile p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return p;
}
char getc(){
    uint64 code = 0x41;
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    uint64 volatile p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return (char)p;
}

void putc(char c){
    uint64 code = 0x42;
    __asm__ volatile("mv a1, %0" : : "r" (c));
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
}

int get_thread_id(){
    uint64 code = 0x16;
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    uint64 volatile p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return (int)p;
}

int thread_delete(thread_t handle){
    uint64 code = 0x17;
    if(!handle) return -1;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    return 0;
}

int kill(thread_t handle){
    uint64 code = 0x50;
    if(!handle) return -1;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    uint64 volatile p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return (int)p;
}

int thread_join(thread_t* handle){
    uint64 code = 0x51;
    if(!*handle) return -1;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r" (code));
    __asm__ volatile("ecall");
    uint64 volatile p;
    __asm__ volatile("mv %0, a0" : "=r"(p));
    return (int)p;
}