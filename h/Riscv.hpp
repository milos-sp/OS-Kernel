//
// Created by os on 5/19/22.
//

#ifndef PROJECT_BASE_RISCV_HPP
#define PROJECT_BASE_RISCV_HPP


#include "../lib/hw.h"
#include "MemoryAllocator.hpp"
#include "PCB.hpp"
#include "SemDescription.hpp"


class Riscv{
public:
    static void trap();

    static void popSppSpie();

    static uint64 previousPrivilege(uint64 sstatus);

    static void prevPrivilege();

    static void  handleTrap();

    static uint64 r_scause();

    static void w_scause(uint64 scause);

    static uint64 r_sepc();

    static void w_sepc(uint64 sepc);

    static uint64 r_stvec();

    static void w_stvec(uint64 stvec);

    static uint64 r_stval();

    static void w_stval(uint64 stval);

    enum BitMaskSIP{
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9)
    };

    static void ms_sip(uint64 mask); //mask
    static void mc_sip(uint64 mask); //clear

    static uint64 r_sip();
    static void w_sip(uint64 sip);

    enum BitMaskSSTATUS{
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8)
    };

    static void ms_sstatus(uint64 mask); //mask
    static void mc_sstatus(uint64 mask); //clear

    static uint64 r_sstatus();
    static void w_sstatus(uint64 sstatus);

    static void moveArguments(uint64 *a){ //ucitavanje argumenata sistemskog poziva
        __asm__ volatile("ld %0, 80(s0)" : "=r"(a[0]));
        __asm__ volatile("ld %0, 88(s0)" : "=r"(a[1]));
        __asm__ volatile("ld %0, 96(s0)" : "=r"(a[2]));
        __asm__ volatile("ld %0, 104(s0)" : "=r"(a[3]));
        __asm__ volatile("ld %0, 112(s0)" : "=r"(a[4]));
    }

    static void w_a0(uint64 p);
};


//inline?
inline uint64 Riscv::r_scause()
{
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause)
{
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 Riscv::r_sepc()
{
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void Riscv::w_sepc(uint64 sepc)
{
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 Riscv::r_stvec()
{
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void Riscv::w_stvec(uint64 stvec)
{
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 Riscv::r_stval()
{
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void Riscv::w_stval(uint64 stval)
{
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void Riscv::ms_sip(uint64 mask)
{
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sip(uint64 mask)
{
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sip()
{
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void Riscv::w_sip(uint64 sip)
{
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void Riscv::ms_sstatus(uint64 mask)
{
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask)
{
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sstatus()
{
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void Riscv::w_sstatus(uint64 sstatus)
{
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

inline void Riscv::w_a0(uint64 p){
    __asm__ volatile("mv a0, %0" : : "r"(p)); //potrebna za ucitavanje rezultata pozi kernelske funkcije u registar a0
}



#endif //PROJECT_BASE_RISCV_HPP
