//
// Created by os on 8/10/22.
//
#include "../h/syscall_cpp.hpp"

char Console::getc() { //samo se pozivaju oni iz syscall_c
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}