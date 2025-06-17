#ifndef _MY_PROCESSGENERATOR_H_
#define _MY_PROCESSGENERATOR_H_

#include "my_PCB.h"
#include "my_processQueue.h"
#include "my_clocktimer.h" //necesario para acceder a la estructura my_machine declarada como variable global en el main

#define QUANTUM 10 //quantum fijo para todos los procesos generados

/*********************** FUNCIONES DECLARADAS ***********************/
void my_processGenerator();


#endif