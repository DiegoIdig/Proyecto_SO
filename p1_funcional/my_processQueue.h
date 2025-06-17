#ifndef _MY_PQUEU_H_
#define  _MY_PQUEU_H_

#include <stdio.h>

#include "my_PCB.h"

//#define PROCESS_MAX_NUM 20;

struct nodo
{
    struct my_PCB* pcb;
    struct nodo* next;
    struct nodo* prev;
};

struct my_processQueue
{
    //puntero a primer PCB
    struct nodo *first;
    struct nodo *last;
    int totalProcess;
    int processNumLimit;
    int cpu_id; //id del procesador al que corresponde la cola RR
};

extern struct my_processQueue **global_queues;

/*********************** FUNCIONES DECLARADAS ***********************/
struct my_processQueue* create_my_processQueue(int maxPermitedProcess, int cpu_id);
int add_process(struct my_processQueue* pQueue, struct my_PCB* newPCB);
struct nodo* search_Process(struct my_processQueue* pQueue, struct my_PCB* pcbToSearch);
int remove_Process(struct my_processQueue* pQueue, struct my_PCB* pcbToRemove);
void print_queue(struct my_processQueue* pQueue);

#endif // _MY_PQUEU_H_