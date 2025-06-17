#ifndef _MY_CLOCKTIMER_C_
#define _MY_CLOCKTIMER_C_

#define LOAD_VALUE 0x000000
#define STORE_VALUE 0x100000
#define ADD_VALUE 0x200000
#define EXIT_VALUE 0xF00000

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "my_scheduler_dispatcher.h"

struct my_thread
{
    int core_id;
    int cpu_id;
    int thread_id;
    int pc;
    int ir;
    uint32_t* ptbr; //puntero a la tabla de paginas del proceso en la memoria fisica
    //MMU->TLB
};

struct my_core
{
    int id;
    int cpu_id;
    int numThreads;
    struct my_thread **threads;
};

struct my_CPU
{
    int id;
    int numCores;
    struct my_core **cores; //array de punteros a structs tipo my_core
};

struct my_machine
{
    int numCPU;
    struct my_CPU **array_cpu;
    long int global_cycles;
};

extern struct my_machine* machine; //se declara como variable global en el main

//Para el clock
struct clock_params
{
    int totalTimers;
    struct my_machine *machine;
};

//Para el timer
struct timer_params
{
    int frequency;
    void (*callback)(void);  //puntero a función que recibe argumentos y no retorna nada.
    //void *callback_arg; //argumentos para la función callback
};


/*********************** FUNCIONES DECLARADAS ***********************/
struct my_machine* create_my_machine(int numCPU, int numCores, int numThreads);
void* my_clock(void *arg);
void* my_timer(void *arg);
void execute_process(struct my_PCB* pcb)

#endif