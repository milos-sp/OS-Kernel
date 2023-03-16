#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
#include "../h/PCB.hpp"

#include "../test/printing.hpp"
#include "../h/userMain.hpp"

// \n

void userWrapper(void *sem) //mora wrapper posto je userMain bez argumenata
{
    printString("userMain started!\n");
    userMain();
    printString("userMain finished!\n");
    sem_signal((sem_t)sem);
}


int main(){

    PCB* kernel = PCB::kernelThreadMaker();

    Riscv::w_stvec((uint64)&Riscv::trap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    printString("Main started!\n");

    sem_t user_sem;
    sem_open(&user_sem, 0);

    thread_t user;
    thread_create(&user, userWrapper, user_sem);

    sem_wait(user_sem);//kad se javi signal prolazi i gasi semafor
    sem_close(user_sem);

    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    delete kernel;
    delete user;
    printString("Main finished!\n");

    return 0;
}
