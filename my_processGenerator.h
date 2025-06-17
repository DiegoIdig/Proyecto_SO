#ifndef _MY_PROCESSGENERATOR_H_
#define _MY_PROCESSGENERATOR_H_

#include "my_PCB.h"
#include "my_processQueue.h"
#include "my_clocktimer.h" //necesario para acceder a la estructura my_machine declarada como variable global en el main
#include "physical_memory.h"

#define QUANTUM 10 //quantum fijo para todos los procesos generados
#define MAX_LINE_SIZE 128 //tamanio maximo de nombre de archivo para el loader

/*********************** FUNCIONES DECLARADAS ***********************/
void my_processGenerator();
int loader(const char* filename, struct my_PCB* pcb);

#endif