//
// Created by os on 5/12/22.
//

#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"


MemBlock *MemoryAllocator::freeMemHead = nullptr;

bool MemoryAllocator::first = true;

void *MemoryAllocator::mem_alloc(size_t size) {
    size_t sz = size / MEM_BLOCK_SIZE;
    if(size % MEM_BLOCK_SIZE) { sz++; size = sz * MEM_BLOCK_SIZE; }
    if(first){ //potrebno je postaviti ispravno memoriju kada se prvi put pozove alokacija
        first = false;
        freeMemHead = (MemBlock*)HEAP_START_ADDR;
        freeMemHead->prev = freeMemHead->next = nullptr;
        freeMemHead->size = (size_t)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - MEM_BLOCK_SIZE);
    }
    MemBlock *block = freeMemHead;
    while(block != nullptr) {
        if(block->size > size) {
            if (block->size == size) { //pronadjen komad iste velicine
                removeNode(&freeMemHead, block);

                return ((char *) (block) + MEM_BLOCK_SIZE);
            }  // ako nije tačna, napravi novi segment posle size prostora, i ubaci ga u free listu
            removeNode(&freeMemHead, block);
            size_t remSize = block->size - size - MEM_BLOCK_SIZE;
            if(remSize >= MEM_BLOCK_SIZE) { //ako je preostala memorija manja od zaglavlja ne ubacuje se
                MemBlock *newBlk = (MemBlock *) ((char *) block + size + MEM_BLOCK_SIZE);
                newBlk->size = remSize;
                newBlk->prev = newBlk->next = nullptr;
                addNode(&freeMemHead, newBlk);
            }
            block->size = size;

            return ((char*)block + MEM_BLOCK_SIZE);
        }
        block = block->next;
    }
    return nullptr;
}

int MemoryAllocator::mem_free(void *addr) {
    if(!addr) return -1;
    MemBlock* address = (MemBlock*)((char*)addr - MEM_BLOCK_SIZE);

    addNode(&freeMemHead, address);
    //treba odraditi spajanje sa susednim slobodnim segmentima ako postoje
    MemBlock* next, *curr;
    if(address->prev) curr = address->prev;
    else curr = address;
    while(curr->next && (((char*)curr + curr->size + MEM_BLOCK_SIZE >= ((char*)curr->next)))){
        next = curr->next;
        curr->size += next->size + MEM_BLOCK_SIZE;
        curr->next = curr->next->next;
        if(curr->next) curr->next->prev = curr;
    }
    return 0;
}

void MemoryAllocator::addNode(MemBlock **head, MemBlock *blk)
{
    if(blk == nullptr) return;

    //umetanje u listu prvi put, ako je lista null
    if((*head) == nullptr){
        *head = blk; return;
    }
    //ako treba umetnuti na adresi ispred koje pokazuje glava
    MemBlock* mb = *head;
    if(blk < mb){ //niza adresa nego trenutna glava liste
        blk->next = mb;
        mb->prev = blk;
        *head = blk; //ovo opet
        return;
    }
    //ako treba umetnuti negde u listi, potreban je prolaz kroz listu preko petlje
    MemBlock* prev = mb;
    for(MemBlock* curr = mb->next; curr != nullptr; curr = curr->next){
        if(blk < curr){
            blk->prev = prev;
            blk->next = curr;
            prev->next = blk;
            curr->prev = blk;
            return;
        }else prev = curr;
    }
    //umetanje na sam kraj
    blk->prev = prev;
    prev->next = blk;
}

void MemoryAllocator::removeNode(MemBlock **head, MemBlock *blk)
{
    if(!blk || !head) return;
    MemBlock* htemp = *head;
    if(htemp == blk){ //uklanja se trenutna glava
        *head = htemp->next;
        if(*head != nullptr){
            (*head)->prev = nullptr; //prvi nema prethodnika!
        }
        blk->next = blk->prev = nullptr;
        return;
    }
    //u suprotnom je potrebno prevezati cvorove
    MemBlock *prev = blk->prev, *next = blk->next;
    prev->next = next;
    if(next) next->prev = prev;
    blk->next = blk->prev = nullptr;
}

