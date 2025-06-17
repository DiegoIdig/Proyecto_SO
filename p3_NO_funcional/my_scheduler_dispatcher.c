#include "my_scheduler_dispatcher.h"

/*void global_scheduler() {
  pthread_t *scheduler_threads = malloc(machine->numCPU * sizeof(pthread_t));

  printf("---SCHEDULER: iniciando scheduler global\n");
  for (int i=0; i<machine->numCPU; i++) {
    pthread_create(&scheduler_threads[i], NULL, scheduler__wrapper, global_queues[i]);
  }
  pthread_exit(NULL);
}*/

void global_scheduler()
{
  for (int i=0; i<machine->numCPU; i++) my_scheduler(global_queues[i]);
  printf("\tglobal_scheduler: Scheduler global ejecutado para todas las CPUs.\n");
}

void* scheduler__wrapper(void *arg)
{
  struct my_processQueue *pQueue = (struct my_processQueue *)arg;
  my_scheduler(pQueue);
  return NULL;
}

void my_scheduler(struct my_processQueue *pQueue)
{
  printf("\tEjecutando scheduler para CPU %d\n", pQueue->cpu_id);
  if (pQueue->first == NULL || pQueue->totalProcess == 0) //la cola está vacía
  {
    printf("\t---SCHEDULER: la cola de procesos esta vacia.\n");
    return;
  }

  struct nodo* nodo = pQueue->first;
  struct my_PCB* pcb = nodo->pcb; //proceso en ejecucion
  if  (pcb->executedCycles >= pcb->lifetime) //el proceso ha finalizado
  {
    printf("\t---SCHEDULER: Un proceso ha finalizado, mostrando su informacion y eliminando de la cola RR:...");
    print_my_PCB(pcb);
    remove_Process(pQueue, pcb);
    printf("\n");
  }
  else if (pQueue->totalProcess==1)
  {
    printf("\t---SCHEDULER: solo hay un proceso en la cola, no se rota la cola.\n");
  }
  else if (pcb->taken_quantum >= pcb->quantum)
  {
    printf("\t---SCHEDULER: un proceso con pid=%d ha llegado a la cuota quantum (%d), moviendo al final de la cola:...", pcb->pid, pcb->quantum);
    pcb->taken_quantum = 0; //reiniciar el contador de ciclos ejecutados antes de llegar al quantum
    rotate_queue(pQueue);
    printf("\n");
  }
  else printf("\t---SCHEDULER: un proceso con pid=%d no ha llegado a la cuota quantum (%d), no se rota la cola.\n", pcb->pid, pcb->quantum);
  printf("Imprimiendo la cola del procesador %d:\n", pQueue->cpu_id);
  print_queue(pQueue);
  return;
}
