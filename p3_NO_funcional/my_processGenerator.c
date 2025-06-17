#include "my_processGenerator.h"

void my_processGenerator()
{
  struct my_PCB* pcb = create_my_PCB(QUANTUM);
  int i, ind, minProccess = INT_MAX;
  char* path = "ficherosElf/prog000.elf";

  //Crear tabla de paginas y segmentos para el proceso
  /*if (loader(path, pcb) == -1)
  {
    printf("---PROCESS_GENERATOR: ERROR: no se ha podido cargar los segmentos del archivo.elf.\n");
    return;
  }*/

  //Asignar el proceso a la CPU con menor cantidad de procesos en la cola
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

int loader(const char* filename, struct my_PCB* pcb)
{
  FILE* fp = fopen(filename, "r");
  if (fp == NULL)
  {
    printf("ERROR (loader): no se puede abrir el archivo ELF");
    return -1;
  }

  char line[MAX_LINE_SIZE];
  char label[16];
  uint32_t text_vaddr = 0, data_vaddr = 0; //direcciones virtuales del segmento de texto y datos

  //leer la primera linea: .text
  if (fgets(line, sizeof(line), fp) == NULL)
  {
    printf("ERROR (loader): no se pudo leer la linea .text\n");
    fclose(fp);
    return -1;
  }
  int res = sscanf(line, "%s %x", label, &text_vaddr); //sscanf devuelve el numero de items o matches que encuentra al escanear la linea indicada: en este caso debe encontrar 2 (etiqueta y direccion virutal) 
  if (res != 2 || strcmp(label, ".text") != 0)
  { //si no se ha obtenido 2 items (etiqueta y vaddr) o la etiqueta no es ".text" -> mensaje de error
    printf("ERROR (loader): formato incorrecto en la linea .text\n");
    fclose(fp);
    return -1;
  }

  //de igual forma para el segmento de .data
  if (!fgets(line, sizeof(line), fp))
  {
    printf("ERROR (loader): no se pudo leer la linea .data\n");
    fclose(fp);
    return -1;
  }
  res = sscanf(line, "%s %x", label, &data_vaddr);
  if (res != 2 || strcmp(label, ".data") != 0)
  {
    printf("ERROR (loader): formato incorrecto en la linea .data\n");
    fclose(fp);
    return -1;
  }

  //calcular la cantidad de palabras que debe tener el segmento .text
  size_t text_size = data_vaddr - text_vaddr;
  size_t text_words = text_size / WORD_SIZE;  // WORD_SIZE == 4
    
  size_t total_words = 0;
  while(fgets(line, sizeof(line), fp)) total_words++;

  //posicion al inicio del fichero y saltar las dos primeras lineas para empezar a leer el segmento de texto
  uint32_t *buffer = malloc(total_words * sizeof(uint32_t));
  size_t i = 0;
  char* nl;
  uint32_t word_val;
  fseek(fp, 0, SEEK_SET);
  fgets(line, sizeof(line), fp);
  fgets(line, sizeof(line), fp);

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    //quitar el salto de linea
    nl = strchr(line, '\n');
    if (nl != NULL) *nl = '\0'; //cambiamos el caracter \n por caracter nulo \0 (fin de string/cadena de texto)

    if (sscanf(line, "%x", &word_val) != 1)
    {
      printf("ERROR (loader): al obtener el valor de la linea: %s\n", line);
      free(buffer);
      fclose(fp);
      return -1;
    }
    buffer[i] = word_val;
    i++;
  }
  fclose(fp);

  //actualizar el PCB con los punteros a direcciones virtuales de cada segmento
  pcb->mm->code = text_vaddr;
  pcb->mm->data = data_vaddr;

  //crear tabla de paginas y cargar segmentos .text y .data en memoria fisica
  size_t data_words = total_words - text_words;
  load_segments(pcb, buffer, text_words, data_words);

  free(buffer);
  printf("---PROCESS_GENERATOR (loader): segmentos cargados en memoria fisica.\n");
  return 0;
}

