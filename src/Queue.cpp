//
// Created by os on 8/13/22.
//
#include "../h/Queue.hpp"

void Queue::put(PCB *data)
{
    Elem* elem = new Elem(data, nullptr);
    if(tail){
        tail->next = elem;
        tail = elem;
    }else{
        head = tail = elem;
    }
    counter++;
}

PCB *Queue::get()
{
    if(head == nullptr) return nullptr;
    Elem* del = head;
    head = head->next;
    if(head == nullptr) tail = nullptr;
    PCB* data = del->data;
    counter--;
    delete del;
    return data;
}
