//
// Created by os on 8/11/22.
//
#include "../h/syscall_cpp.hpp"

Thread::Thread (void (*body)(void*), void* arg){
    thread_create_without_start(&myHandle, body, arg);
}
Thread::~Thread(){
    thread_delete(myHandle);
}
Thread::Thread() {
    thread_create_without_start(&myHandle, Thread::threadWrapper, this); //za this se pokrece run
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::start() {
    return thread_start(myHandle);
}

void Thread::threadWrapper(void *arg) {
    ((Thread*)arg)->run();
}

int Thread::getThreadId() {
    return get_thread_id();
}

void Thread::join() {
    thread_join(&myHandle);
}
