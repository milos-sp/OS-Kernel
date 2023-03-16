//
// Created by os on 7/29/22.
//

#ifndef PROJECT_BASE_LIST_HPP
#define PROJECT_BASE_LIST_HPP
#include "MemoryAllocator.hpp"

template<typename T>

class List{
private:
    struct Elem{
        T* data;
        Elem* next;

        Elem(T* data, Elem* next) : data(data), next(next){}
        void *operator new(size_t size) { return MemoryAllocator::mem_alloc(size); }
        void operator delete(void *ptr) { MemoryAllocator::mem_free(ptr); }
    };

    Elem *head, *tail;


public:
    List(){
        head = tail = nullptr;
    }
    List(const List<T>&) = delete;
    List<T>& operator =(const List<T>&) = delete;

    void addFront(T* data){
        Elem* elem = new Elem(data, head);
        head = elem;
        if(!tail) tail = head;

    }

    void addEnd(T* data){
        Elem* elem = new Elem(data, nullptr);
        if(tail){
            tail->next = elem;
            tail = elem;
        }else{
            head = tail = elem;
        }

    }

    T* removeFront(){
        if(head == nullptr) return nullptr;
        Elem* del = head;
        head = head->next;
        if(head == nullptr) tail = nullptr;
        T* data = del->data;

        delete del;
        return data;
    }

    T* peekFront(){
        if(head == nullptr) return nullptr;
        return head->data;
    }

    T* removeEnd(){
        if(head == nullptr) return nullptr;
        Elem* prev = nullptr;
        for(Elem* curr = head; curr && curr != tail; curr = curr->next){ prev = curr; }
        Elem* del = tail;
        if(prev) prev->next = nullptr;
        else head = nullptr;
        tail = prev;

        T* data = del->data;
        delete del;
        return data;
    }

    T* peekEnd(){
        if(tail == nullptr) return nullptr;
        else return tail->data;
    }

};

#endif //PROJECT_BASE_LIST_HPP
