#include "my_scheduler_dispatcher.h"

void my_scheduler(struct my_processQueue *pQueue)
{
  printf("Ejecutando scheduler para CPU %d\n", pQueue->cpu_id);
  struct nodo *nodo = pQueue->first;
  struct my_PCB pcb = nodo->pcb; //proceso en ejecucion
  if  (pcb.executedCycles > pcb.lifetime) //el proceso ha finalizado
  {
    printf("---SCHEDULER: Un proceso ha finalizado, mostrando su informacion y eliminando de la cola RR:...");
    print_my_PCB(&pcb);
    remove_Process(pQueue, &pcb);
    printf("\n");
  }
  else
  {
    printf("---SCHEDULER: un proceso con pid=%ld ha llegado a la cuota quantum (%d), moviendo al final de la cola:...\n", pcb.pid, pcb.quantum);
    add_process(pQueue, pcb);
    remove_Process(pQueue, &pcb);
    printf("\n");
  }
  print_queue(pQueue);
  //printf("\n\n SCHEDULER \n\n");
}
