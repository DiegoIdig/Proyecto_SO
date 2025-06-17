#include "my_clocktimer.h"
#include "my_processGenerator.h"

struct my_machine* create_my_machine(int numCPU, int numCores, int numThreads)
{
  int i, j, k;
  struct my_machine* machine = malloc(sizeof(struct my_machine));
  int global_process_id = 0;
  
  machine->global_cycles = 0;
  machine->numCPU = numCPU;
  machine->array_cpu = malloc(numCPU * sizeof(struct my_CPU*));
  for (i = 0; i < numCPU; i++)
  {
    machine->array_cpu[i] = malloc(sizeof(struct my_CPU));
    machine->array_cpu[i]->id = i;
    machine->array_cpu[i]->numCores = numCores;
    machine->array_cpu[i]->cores = malloc(numCores * sizeof(struct my_core*));
    
    for (j = 0; j < numCores; j++)
    {
      machine->array_cpu[i]->cores[j] = malloc(sizeof(struct my_core));
      machine->array_cpu[i]->cores[j]->id = j;
      machine->array_cpu[i]->cores[j]->cpu_id = i;
      machine->array_cpu[i]->cores[j]->numThreads = numThreads;
      machine->array_cpu[i]->cores[j]->threads = malloc(numThreads * sizeof(struct my_thread*));

      for (k = 0; k < numThreads; k++)
      {
        machine->array_cpu[i]->cores[j]->threads[k] = malloc(sizeof(struct my_thread));
        machine->array_cpu[i]->cores[j]->threads[k]->core_id = j;
        machine->array_cpu[i]->cores[j]->threads[k]->cpu_id = i;
        machine->array_cpu[i]->cores[j]->threads[k]->thread_id = global_process_id;
        global_process_id++;
      }
    }
  }
  printf("Creacion de la maquina finalizada.\n");
  return machine;
}

//Variables globales para la sincronizacion entre clock y timers
int done = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //cerrojo para sincronizar clock y timer
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int stop_system = 0;

void* my_clock(void *arg)
{
  struct clock_params *params = (struct clock_params *) arg;
  int totalTimers = params->totalTimers;
  struct my_machine *machine = params->machine;

  printf("SYSTEM: Reloj en marcha.\n");
  //printf("Dentro de my_clock: totalTimers = %d y maquina en: %p\n", params->totalTimers, (void*) params->machine);

  //while(1)
  while(machine->global_cycles < 100)
  {
    pthread_mutex_lock(&mutex);
   
    while(done < totalTimers)
    {
      pthread_cond_wait(&cond, &mutex);
      //printf("DONE:%d\n",done);
      fflush(stdout);
    }

    //printf("\tSYSTEM: StopSystem=%d\n", stop_system);

    machine->global_cycles++;
    struct my_processQueue* pQueue;
    struct nodo* node;
    for (int i=0; i<machine->numCPU; i++)
    {
      pQueue = global_queues[i];
      if (pQueue->first != NULL && pQueue->totalProcess > 0)
      {
        node = pQueue->first;
        node->pcb->executedCycles++; //incrementar el contador de ciclos totales ejecutados del PCB
        node->pcb->taken_quantum++; //incrementar el contador de ciclos ejecutados del PCB antes de llegar al quantum
      }
    }

    done = 0; //reiniciar contador de done
    printf("SYSTEM: Reloj ciclos del sistema: %ld\n", machine->global_cycles);
    done=0;

    pthread_cond_broadcast(&cond2);
    pthread_mutex_unlock(&mutex);
  }
  printf("SYSTEM: Reloj ha alcanzado el maximo de ciclos, APAGANDO SISTEMA.\n");
  stop_system = 1;
  pthread_cond_broadcast(&cond2);
  return NULL;
}

void* my_timer(void *arg)
{
  struct timer_params *params = (struct timer_params *) arg;
  int frequency = params->frequency;
  int cont = 0;

  printf("SYSTEM: Timer en marcha con frecuencia=%d.\n", frequency);

  //while(1)
  while(!stop_system)
  {
    pthread_mutex_lock(&mutex);
    
    done++;
    //printf("  Timer working, done = %i.\n", done);
    cont++;
    //printf("  Cont: %i.\n", cont);
    if (cont == frequency)
    {
      printf("\t---Cont=%d, ejecutando funcion correspondiente al timer\n", cont);
      cont = 0;
      //ejecutar la funcion correspondiente del timer
      params->callback();
    }
    //printf("Timer done\n");

    pthread_cond_signal(&cond);
    pthread_cond_wait(&cond2, &mutex);

    pthread_mutex_unlock(&mutex);
  }
  printf("SYSTEM: Timer finalizando.\n");
  return NULL;
}
