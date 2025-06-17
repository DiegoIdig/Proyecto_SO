#include "my_PCB.h"

int pid = 0;

struct my_PCB create_my_PCB()
{
  //TO-DO
  //Tienes que comunicarlo de alguna forma con el clock, para saber cuantos ciclos de ejecución ha realizado cada proceso
  //ese valor imagino que lo puedes guardar en el propio PCB y cuando llegue al quantum ...

  struct my_PCB pcb;
  
  pcb.lifetime = rand();
  pcb.pid = pid;
  pid++;
  pcb.quantum = rand() % 10; //por ejemplo, entre 1 y 10
  pcb.executedCycles = 0;

  printf("PCB generated.\n---Quantum: %i \t pid: %i lifetime: %i\n", pcb.quantum, pcb.pid, pcb.lifetime);

  return pcb;
}

void print_my_PCB(struct my_PCB* pcb)
{
  printf("----------PCB----------\nid=%i, quantum=%i, executedCycles=%d, lifetime=%d\n", pcb->pid, pcb->quantum, pcb->executedCycles, pcb->lifetime);
}
