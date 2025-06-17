#include "my_processGenerator.h"

void my_processGenerator()
{
  struct my_PCB* pcb = create_my_PCB(QUANTUM);
  int i, ind, minProccess = INT_MAX;
  ind = -1; 
  for (i=0; i<machine->numCPU; i++)
  {
    if (global_queues[i]->totalProcess < minProccess)
    {
      minProccess = global_queues[i]->totalProcess;
      ind = i;
    }
  }
  if (ind!=-1) printf("---PROCESS_GENERATOR: Proceso generado, anadiendo a la cola de CPU %d\n", ind);
  print_my_PCB(pcb);
  add_process(global_queues[ind], pcb);
}
