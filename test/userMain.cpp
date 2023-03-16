//#include "../test/Threads_C_API_test.hpp" // zadatak 2, niti C API i sinhrona promena konteksta
//#include "../test/Threads_CPP_API_test.hpp" // zadatak 2., niti CPP API i sinhrona promena konteksta

//#include "../test/ConsumerProducer_C_API_test.h" // zadatak 3., kompletan C API sa semaforima, sinhrona promena konteksta
#include "../test/ConsumerProducer_CPP_Sync_API_test.hpp" // zadatak 3., kompletan CPP API sa semaforima, sinhrona promena konteksta

//#include "../test/ThreadSleep_C_API_test.hpp" // thread_sleep test C API
//#include "../test/ConsumerProducer_CPP_API_test.hpp" // zadatak 4. CPP API i asinhrona promena konteksta

#include "../h/PCB.hpp"

struct args{
    sem_t user_sem, mutex;
    char id;
};

const int workerNumber = 3;
const int workerCount = 10;
const int timeout = 5;

int testCnt = 0;

void worker(void* arg){
    args* a = (args*)(arg);
    for(int i = 0; i < workerCount; i++) {
        while(PCB::timeSliceCounter < timeout);
        testCnt++;
        PCB::timeSliceCounter = 0;
        if(testCnt == 15){
            sem_signal(a->user_sem);
            kill(PCB::running);
        }
        sem_wait(a->mutex); //atomicnost

        int id = get_thread_id();
        char w = a->id;
        putc(w);
        putc(':');
        printInt(id);
        putc(',');
        sem_signal(a->mutex);
    }
    sem_signal(a->user_sem);
    delete a;
}

void employer(void* arg){
    printString("Employer started!\n");
   // sem_t userSem = (sem_t)(arg);
    sem_t user_sem, mutex;
    sem_open(&user_sem, 0);
    sem_open(&mutex, 1);
    thread_t workers[workerNumber];
    for(int i = 0; i < workerNumber; i++){
        args* a = new args{user_sem, mutex, (char)('A' + i)};
        thread_create_without_start(&workers[i], worker, a);
    }
    for(int i = 0; i < workerNumber; i++){
        thread_start(workers[i]);
    }
   // wait_for_all();
    /*sem_wait(user_sem);
    sem_wait(user_sem);
    sem_wait(user_sem);*/
    thread_join(&workers[0]);
    thread_join(&workers[1]);
    thread_join(&workers[2]);
    putc('\n');
    printString("Employer finished!\n");
   // sem_signal(userSem);
}

Thread* workers[3];

struct targs{
    Semaphore* userSem;
    char id;
};

class WorkerCPP : public Thread{
    targs* a;

public:

    WorkerCPP(targs* ar) : Thread(), a(ar){}

    void run() override{
        for(int i = 0; i < 10; i++){
            while(PCB::timeSliceCounter < 5);
            //PCB::timeSliceCounter = 0;
            int id = Thread::getThreadId();
            char w = a->id;
            if(w == 'A'){
                workers[1]->join();
                workers[2]->join();
            }
            Console::putc(w);
            Console::putc(':');
            printInt(id);
            Console::putc(',');
        }
        a->userSem->signal();
    }
};

class EmployerCPP : public Thread{
    Thread** workers;
public:
    EmployerCPP(Thread** work) : Thread(){}

    void run() override{
        printString("Employer started!\n");
        workers[0]->join();
        workers[1]->join();
        workers[2]->join();
        printString("Employer finished!\n");
    }
};


void userMain() {
    //Threads_C_API_test(); // zadatak 2., niti C API i sinhrona promena konteksta
    //Threads_CPP_API_test(); // zadatak 2., niti CPP API i sinhrona promena konteksta
    //producerConsumer_C_API(); // zadatak 3., kompletan C API sa semaforima, sinhrona promena konteksta
    //producerConsumer_CPP_Sync_API(); // zadatak 3., kompletan CPP API sa semaforima, sinhrona promena konteksta

    //testSleeping(); // thread_sleep test C API
    //ConsumerProducerCPP::testConsumerProducer(); // zadatak 4. CPP API i asinhrona promena konteksta, kompletan test svega
   /* sem_t user_sem, mutex;
    sem_open(&user_sem, 0);
    sem_open(&mutex, 1);
    thread_t workers[workerNumber];
    for(int i = 0; i < workerNumber; i++){
        args* a = new args{user_sem, mutex, (char)('A' + i)};
        thread_create(&workers[i], worker, a);
    }
    thread_join(workers[0]); //workers[0].join(); valjda za cpp
    thread_join(workers[1]);
    thread_join(workers[2]);
    //sem_wait(user_sem);
    //sem_wait(user_sem);
    //sem_wait(user_sem);
    putc('\n');*/
    /*thread_t empl;
    //sem_t userSem;
    //sem_open(&userSem, 0);
    thread_create(&empl, employer, nullptr);
   // wait_for_all();
    thread_join(empl);
    //sem_wait(userSem);
    //sem_close(userSem);
    putc('\n');*/
    //cpp
    Semaphore *user = new Semaphore(0);
    //Thread* workers[3];
    //Thread* empl = new EmployerCPP(workers);
    //empl->start();

    for(int i = 0; i < 3; i++){
        targs* a = new targs{user, (char)('A' + i)};
        workers[i] = new WorkerCPP(a);
        workers[i]->start();
    }
    //empl->join();
    /*user->wait();
    user->wait();
    user->wait();*/
    workers[0]->join();
    workers[1]->join();
    workers[2]->join();
    putc('\n');
}