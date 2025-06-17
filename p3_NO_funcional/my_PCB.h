#ifndef _MY_PCB_H_
#define _MY_PCB_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define MAX_EXECUTION_TIME 300 //300 ciclos como maximo de lifetime

struct mm
{
    uint32_t data; //direccion del segmento de datos del programa a ejecutar
    uint32_t code; //direccion del segmento de código e instrucciones del programa a ejecutar
    uint32_t pgb; //direccion fisica de la tabla de paginas
};

struct my_PCB
{
    //pthread_t pid;
    int pid;
    int lifetime; //tiempo de vida del proceso, aleatorio
    int quantum; //generado aleatoriamente con rand
    int executedCycles; //numero de ciclos total que ha ejecutado el proceso en la CPU
    int taken_quantum; //numero de ciclos que ha ejecutado el proceso en la CPU antes de llegar al quantum
    struct mm* mm; //puntero a estructura tipo mm
};


/*********************** FUNCIONES DECLARADAS ***********************/
struct my_PCB* create_my_PCB(int quantum);
void print_my_PCB(struct my_PCB* pcb);
void free_my_PCB(struct my_PCB* pcb);

#endif