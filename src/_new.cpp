//
// Created by os on 6/30/22.
//
#include "../h/syscall_c.h"
//#include "../lib/mem.h"

void* operator new (size_t size){return mem_alloc(size);}
void* operator new [](size_t size){return mem_alloc(size);}
void operator delete (void* adr) noexcept { mem_free(adr);}
void operator delete [](void* adr) noexcept { mem_free(adr);}


