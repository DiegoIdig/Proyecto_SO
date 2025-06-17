#include "my_processQueue.h"

struct my_processQueue create_my_processQueue(int maxPermitedProcess, int cpu_id)
{
    struct my_processQueue pQueue;
    pQueue.first = NULL;
    pQueue.last = NULL;
    pQueue.totalProcess = 0;
    pQueue.processNumLimit = maxPermitedProcess;
    pQueue.cpu_id = cpu_id;
    return pQueue;
}


int add_process(struct my_processQueue* pQueue, struct my_PCB newPCB)
{
    int totalProcess = pQueue->totalProcess;
    int processNumLimit = pQueue->processNumLimit;

    if (totalProcess < processNumLimit)
    {
        struct nodo nodo;
        nodo.pcb = newPCB;

        if (pQueue->first == NULL) //es el primer proceso de la cola
        {
            nodo.prev = NULL;
            nodo.next = NULL;
            pQueue->last = pQueue->first;
            pQueue->totalProcess++;
        }
        else
        {
            struct nodo* lastNode = pQueue->last;
            nodo.prev = pQueue->last;
            nodo.next = pQueue->first;
            pQueue->last->next = nodo;

            pQueue->totalProcess++;
        }
        printf("Process added to queue.\n");
        return 1;
    }
    else
    {
        printf("\n\nERROR: maximum process instances reached.\n\n");
        return 0;
    }
}

struct my_PCB* search_Process(struct my_processQueue* pQueue, struct my_PCB* pcbToSearch)
{
    struct my_PCB* actualPCB = pQueue->first;
    if (pQueue->first == pcbToSearch) return actualPCB;
    else if (pQueue->last == pcbToSearch) return pQueue->last;
    else
    {   //si no es ni el primer ni el ultimo elemento, devuelve el elemento anterior al que queremos encontrar
        //esto lo he implementado así para facilitar la implementación de remove_Process
        while (actualPCB->nextProcess != pQueue->last) //mientras no se haya recorrido toda la lista buscamos
        {
            if (actualPCB->nextProcess->pid == pcbToSearch->pid) //PCB encontrado en lista
            {
                return actualPCB;
            }
            //else
            actualPCB = actualPCB->nextProcess; //actualizar puntero actual al siguiente elemento
        }
    }
    return NULL; //no se ha encontrado el elemento
}

void remove_Process(struct my_processQueue* pQueue, struct my_PCB* pcbToRemove)
{
    //Buscamos en la cola el PCB a eliminar
    struct my_PCB* pcbSearched = search_Process(pQueue, pcbToRemove);
    
    if (pcbSearched == NULL)
    {
        printf("ERROR: PCB (id=%lli) to remove dont exists in the queue.\n", pcbToRemove->pid);
        return;
    }

    if (pQueue->first == pcbSearched) //el proceso a eliminar es el primero del RR        
    {
        printf("El proceso ha eliminar es el primero.\n");
        pQueue->first = pcbSearched->nextProcess;
        struct my_PCB *lastPCB = pQueue->last;
        lastPCB->nextProcess = pQueue->first;
    }
    else if (pQueue->last == pcbSearched) //el proceso a eliminar es el ultimo del RR
    {
        printf("El proceso ha eliminar es el ultimo.\n");
        pQueue->first = pcbSearched->nextProcess;
        pQueue->last = pQueue->first;
    }
    else
    {
        printf("El proceso ha eliminar se encuentra en la lista.\n");
        pcbSearched->nextProcess = pcbSearched->nextProcess->nextProcess;
    }
    pQueue->totalProcess--;
}

void print_queue(struct my_processQueue* pQueue)
{
    struct my_PCB* actualPCB = pQueue->first;
    while (actualPCB != pQueue->last) //mientras no se haya recorrido toda la lista buscamos
        {
            print_my_PCB(actualPCB);
            actualPCB = actualPCB->nextProcess; //actualizar puntero actual al siguiente elemento
        }
}


