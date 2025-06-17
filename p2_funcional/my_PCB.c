#include "my_PCB.h"

int pid = 0;

struct my_PCB* create_my_PCB(int quantum)
{
  //TO-DO
  //Tienes que comunicarlo de alguna forma con el clock, para saber cuantos ciclos de ejecución ha realizado cada proceso
  //ese valor imagino que lo puedes guardar en el propio PCB y cuando llegue al quantum ...

  struct my_PCB* pcb = malloc(sizeof(struct my_PCB));
  
  pcb->lifetime = rand() %  MAX_EXECUTION_TIME;
  pcb->pid = pid;
  pid++;
  pcb->executedCycles = 0;
  pcb->quantum = quantum;
  pcb->taken_quantum = 0;

  printf("\tcreate_my_PCB: PCB generado con quantum=%i, pid=%i, lifetime=%i.\n", pcb->quantum, pcb->pid, pcb->lifetime);

  return pcb;
}

void print_my_PCB(struct my_PCB* pcb)
{
  printf("\t---PCB: id=%i, quantum=%i, taken_quantum=%d, executedCycles=%d, lifetime=%d\n", pcb->pid, pcb->quantum, pcb->taken_quantum, pcb->executedCycles, pcb->lifetime);
}
