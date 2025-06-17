#ifndef _MY_CLOCKTIMER_C_
#define _MY_CLOCKTIMER_C_

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "my_scheduler_dispatcher.h"

struct my_thread
{
    int core_id;
    int cpu_id;
    int thread_id;
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

#endif