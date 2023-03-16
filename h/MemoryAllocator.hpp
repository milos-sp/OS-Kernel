//
// Created by os on 5/18/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP
#include "../lib/hw.h"


struct MemBlock {
    size_t size;
    MemBlock *next, *prev;
};


class MemoryAllocator {
private:
    static void addNode(MemBlock **head, MemBlock *blk);

    static void removeNode(MemBlock **head, MemBlock *blk);
public:
    static MemBlock *freeMemHead;

    static bool first;

    static void *mem_alloc(size_t);

    static int mem_free(void*);

};



#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
