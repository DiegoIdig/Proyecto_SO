#ifndef _MY_SCHEDULER_DISPATCHER_H_
#define _MY_SCHEDULER_DISPATCHER_H_

#include <stdio.h>

#include "my_processQueue.h"
#include "my_clocktimer.h" //necesario para acceder a la estructura my_machine declarada como variable global en el main

/*********************** FUNCIONES DECLARADAS ***********************/
void global_scheduler();
void* scheduler__wrapper(void *arg); //wrapper para poder utuilizar pthread_create con my_scheduler desde global_scheduler
void my_scheduler(struct my_processQueue *pQueue);

#endif