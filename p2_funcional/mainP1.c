#include <pthread.h>
#include <stdio.h>

#include "my_clocktimer.h"
#include "my_processQueue.h"
#include "my_processGenerator.h"
#include "my_scheduler_dispatcher.h"

//Variables globales
struct my_processQueue **global_queues; //tantas colas como procesadores
struct my_machine* machine;

int main(int argc, char *argv[])
{

  if (argc != 6)
  {
    printf("Use: %s totalCPUs CoresPerCPU ThreadsPerCore schedulerFrequency processGeneratorFrequency\n", argv[0]);
    return 1;
  }

  int numCPU = atoi(argv[1]);
  int numCoresPerCPU = atoi(argv[2]);
  int numThreadPerCore = atoi(argv[3]);
  int schedulerFreq = atoi(argv[4]);
  int processGenFreq = atoi(argv[5]);
  //otra opcion es tomar valores por entrada estandar

  printf("-----INICIALIZANDO MAQUINA-----\n");
  machine = create_my_machine(numCPU, numCoresPerCPU, numThreadPerCore);

  int totalTimers = 2;
  int maxPermitedProcess = 15;
  global_queues = malloc(numCPU*sizeof(struct my_processQueue*)); //tantas colas como procesadores
  for (int i=0; i<numCPU; i++) global_queues[i] = create_my_processQueue(maxPermitedProcess, i);

  static struct clock_params cp;
  cp.totalTimers = totalTimers;
  cp.machine = machine;
  
  static struct timer_params tpScheduler;
  tpScheduler.frequency = schedulerFreq;
  tpScheduler.callback = global_scheduler;
  //printf("Asignando callback al timer1: %p\n", tpScheduler.callback);
  static struct timer_params tpProcessGen;
  tpProcessGen.frequency = processGenFreq;
  tpProcessGen.callback = my_processGenerator;
  //printf("Asignando callback al timer2: %p\n", tpProcessGen.callback);
  
  pthread_t tid1, tid2, tid3;
  //printf("Direccion de estructura clock_params antes de crear el hilo: %p y de machine: %p\n", &cp, &machine);
  //printf("Totaltimers en cp: %d\n", cp.totalTimers);
  //pthread_create(&tid1, NULL, my_clock, &totalTimers);
  pthread_create(&tid1, NULL, my_clock, &cp);

  //pthread_create(&tid2, NULL, my_timer, &schedulerFreq);
  pthread_create(&tid2, NULL, my_timer, &tpScheduler);
  //pthread_create(&tid3, NULL, my_timer, &processGenFreq);
  pthread_create(&tid3, NULL, my_timer, &tpProcessGen);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
  //pthread_exit(NULL);

  return 0;
}
