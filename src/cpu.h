/*
 * guzhoudiaoke@126.com
 * 2017-11-7
 */

#ifndef _CPU_H_
#define _CPU_H_

#include "kernel.h"
#include "types.h"
#include "syscall.h"
#include "vm.h"
#include "spinlock.h"
#include "timer.h"
#include "list.h"
#include "process.h"
#include "local_apic.h"
#include "tss.h"

#define TRAP_GATE_FLAG      (0x00008f0000000000ULL)
#define INTERRUPT_GATE_FLAG (0x00008e0000000000ULL)
#define SYSTEM_GATE_FLAG    (0x0000ef0000000000ULL)

class cpu_t {
public:
    cpu_t();
    ~cpu_t();

    void init();
    void do_common_isr(trap_frame_t* frame);
    void schedule();
    tss_t* get_tss() { return &m_tss; }
    void update();

    void do_signal(trap_frame_t* frame);

    local_apic_t* get_local_apic();

private:
    void init_gdt();
    void init_idt();
    void init_tss();
    void init_isrs();

    void set_gate(uint32 index, uint32 addr, uint64 flag);
    void set_trap_gate(uint32 index, uint32 addr);
    void set_intr_gate(uint32 index, uint32 addr);
    void set_system_gate(uint32 index, uint32 addr);

    void do_exception(trap_frame_t* frame);
    void do_interrupt(uint32 trapno);
    void do_syscall(trap_frame_t* frame);

private:
    uint64			    m_gdt[GDT_LEN];
    uint64			    m_idt[IDT_LEN];
    tss_t			    m_tss;
    local_apic_t        m_local_apic;
};

#endif
