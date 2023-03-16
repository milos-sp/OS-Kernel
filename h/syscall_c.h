//
// Created by os on 5/18/22.
//

#ifndef PROJECT_BASE_SYSCALL_C_H
#define PROJECT_BASE_SYSCALL_C_H

#include "../lib/hw.h"
//#include "PCB.hpp"
//#include "SemDescription.hpp"

void* mem_alloc(size_t size);
int mem_free(void* adr);

class PCB;
typedef PCB* thread_t;

int thread_create(
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
        );
int thread_exit();
void thread_dispatch();
int thread_create_without_start(
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
        );
int thread_start(thread_t handle);
int get_thread_id();
int thread_delete(thread_t handle);
int kill(thread_t handle);
void wait_for_all();
int thread_join(thread_t* handle);

class SemDescription;
typedef SemDescription* sem_t;

int sem_open(
        sem_t* handle,
        unsigned init
        );
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

typedef unsigned long time_t;
int time_sleep(time_t);

const int EOF = -1;
char getc();
void putc(char);
#endif //PROJECT_BASE_SYSCALL_C_H
