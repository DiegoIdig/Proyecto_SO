#ifndef _MY_PCB_H_
#define _MY_PCB_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct my_PCB
{
    //pthread_t pid;
    int pid;
    int lifetime; //tiempo de vida del proceso, aleatorio
    int quantum; //generado aleatoriamente con rand
    int executedCycles; //numero de ciclos que ha ejecutado el proceso en la CPU
};


/*********************** FUNCIONES DECLARADAS ***********************/
struct my_PCB create_my_PCB();
void print_my_PCB(struct my_PCB* pcb);

#endif