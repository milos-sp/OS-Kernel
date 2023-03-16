//
// Created by os on 8/13/22.
//

#ifndef PROJECT_BASE_QUEUE_HPP
#define PROJECT_BASE_QUEUE_HPP

#include "PCB.hpp"

class Queue {
public:
    struct Elem
    {
        PCB *data;
        Elem *next;

        Elem(PCB *data, Elem *next) : data(data), next(next) {}

        void *operator new(size_t size) { return MemoryAllocator::mem_alloc(size); }
        void operator delete(void *ptr) { MemoryAllocator::mem_free(ptr); }
    };

    Elem *head, *tail;
    int counter;

    Queue() : head(nullptr), tail(nullptr), counter(0) {}

    int size() const { return counter; }

    void put(PCB *data);

    PCB *get();
};

#endif //PROJECT_BASE_QUEUE_HPP
