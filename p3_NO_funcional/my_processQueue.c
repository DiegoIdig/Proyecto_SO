#include "my_processQueue.h"

struct my_processQueue* create_my_processQueue(int maxPermitedProcess, int cpu_id)
{
    struct my_processQueue* pQueue = malloc(sizeof(struct my_processQueue));
    pQueue->first = NULL;
    pQueue->last = NULL;
    pQueue->totalProcess = 0;
    pQueue->processNumLimit = maxPermitedProcess;
    pQueue->cpu_id = cpu_id;
    return pQueue;
}


int add_process(struct my_processQueue* pQueue, struct my_PCB* newPCB)
{
    int totalProcess = pQueue->totalProcess;
    int processNumLimit = pQueue->processNumLimit;

    if (totalProcess < processNumLimit)
    {
        struct nodo* nodo = malloc(sizeof(struct nodo));
        nodo->pcb = newPCB;

        if (pQueue->first == NULL) //es el primer proceso de la cola
        {
            nodo->prev = NULL;
            nodo->next = NULL;
            pQueue->last = nodo;
            pQueue->first = nodo;
        }
        else
        {
            nodo->prev = pQueue->last;
            pQueue->last->next = nodo;
            nodo->next = pQueue->first;
            pQueue->first->prev = nodo;

            pQueue->last = nodo;
        }
        pQueue->totalProcess++;
        printf("\tadd_process: Proceso anadido a la cola.\n");
        return 1;
    }
    else
    {
        printf("\n\nERROR (add_process): la cola de procesos esta llena.\n\n");
        return 0;
    }
}

struct nodo* search_Process(struct my_processQueue* pQueue, struct my_PCB* pcbToSearch)
{
    struct nodo* actualNode = pQueue->first;
    if (pQueue->first == NULL) //la cola está vacía
    {
        printf("ERROR (search_Process): la cola esta vacía.\n");
        return NULL;
    }
    if (pQueue->first->pcb->pid == pcbToSearch->pid) return actualNode;
    else if (pQueue->last->pcb->pid == pcbToSearch->pid) return pQueue->last;
    else
    {
        while (actualNode->next != pQueue->first) //mientras no se haya recorrido toda la lista buscamos
        {
            if (actualNode->pcb->pid == pcbToSearch->pid) //PCB encontrado en lista
            {
                return actualNode;
            }
            //else
            actualNode = actualNode->next; //actualizar puntero actual al siguiente elemento
        }
    }
    return NULL; //no se ha encontrado el elemento
}

int remove_Process(struct my_processQueue* pQueue, struct my_PCB* pcbToRemove)
{
    //Buscamos en la cola el PCB a eliminar
    struct nodo* nodeSearched = search_Process(pQueue, pcbToRemove);
    
    if (nodeSearched == NULL)
    {
        printf("ERROR (remove_Process): el PCB (id=%i) no esta en la cola de procesos.\n", pcbToRemove->pid);
        return 1;
    }

    if (pQueue->totalProcess == 1) //la cola tiene un solo proceso
    {
        printf("\tremove_Process: El proceso a eliminar es el unico de la cola.\n");
        pQueue->first = NULL;
        pQueue->last = NULL;
    }
    else if (pQueue->first == nodeSearched) //el proceso a eliminar es el primero del RR        
    {
        printf("\tremove_Process: El proceso ha eliminar es el primero.\n");
        pQueue->first = nodeSearched->next;
        pQueue->last->next = pQueue->first;
        pQueue->first->prev = pQueue->last;
    }
    else if (pQueue->last == nodeSearched) //el proceso a eliminar es el ultimo del RR
    {
        printf("\tremove_Process: El proceso ha eliminar es el ultimo.\n");
        pQueue->last = nodeSearched->prev;
        pQueue->last->next = pQueue->first;
        pQueue->first->prev = pQueue->last;
    }
    else
    {
        printf("\tremove_Process: El proceso ha eliminar se encuentra en la lista.\n");
        nodeSearched->prev->next = nodeSearched->next;
        nodeSearched->next->prev = nodeSearched->prev;
    }
    pQueue->totalProcess--;
    free_my_PCB(nodeSearched->pcb);
    free(nodeSearched);
    printf("\tremove_Process: Proceso eliminado de la cola.\n");
    return 0;
}

void print_queue(struct my_processQueue* pQueue)
{
    struct nodo* actualNode = pQueue->first;
    if (pQueue->totalProcess == 1)
    {
        print_my_PCB(actualNode->pcb);
        printf("\n");
    }
    else
    {
        while (actualNode->next != pQueue->first) //mientras no se haya recorrido toda la lista buscamos
        {
            print_my_PCB(actualNode->pcb);
            printf("<->");
            actualNode = actualNode->next; //actualizar puntero actual al siguiente elemento
        }
        print_my_PCB(actualNode->pcb); //imprimir ultimo elemento
    }
}

void rotate_queue(struct my_processQueue *pQueue) {
    if (pQueue->first == NULL || pQueue->first == pQueue->last) return; //la cola está vacía o tiene un solo nodo.

    pQueue->first = pQueue->first->next;
    pQueue->last = pQueue->first->prev;
}

