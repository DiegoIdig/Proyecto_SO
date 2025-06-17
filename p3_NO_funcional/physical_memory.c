// physical_memory.c
#include "physical_memory.h"

uint32_t kernel_pageTable_ind = 0;//indice de la siguiente tabla de paginas del kernel disponible

uint32_t* create_physical_memory()
{
    physical_memory = malloc(PHYSICAL_MEMORY_SIZE);
    if (physical_memory == NULL)
    {
        printf("ERROR (init_physical_memory): no es posible reservar memoria fisica\n");
        return NULL;
    }
    memset(physical_memory, 0, PHYSICAL_MEMORY_SIZE);
    return physical_memory;
    //physical_memory = calloc(PHYSICAL_MEMORY_SIZE, 1);
}

uint32_t* create_bitmap()
{
    pages_bitmap = malloc(PHYSICAL_MEMORY_SIZE);
    if (pages_bitmap == NULL)
    {
        printf("ERROR (create_bitmap): no es posible reservar memoria para el bitmap\n");
        return NULL;
    }
    for (int i=0; i<KERNEL_PAGES; i++) pages_bitmap[i] = 1;
    for (int i=KERNEL_PAGES; i<NUM_PAGES; i++) pages_bitmap[i] = 0;
    return pages_bitmap;
}

void free_physical_memory(uint32_t* physical_memory)
{
    free(physical_memory);
    physical_memory = NULL;
}

void free_bitmap(int* pages_bitmap)
{
    free(pages_bitmap);
    pages_bitmap = NULL;
}

uint32_t read_word(uint32_t address)
{
    // Verificar que la direccion esta dentro del rango.
    if(address >= (PHYSICAL_MEMORY_WORDS))
    {
        printf("ERROR (read_word): direccion fuera de rango\n");
        return -1;
    }
    return physical_memory[address];
}

int write_word(uint32_t address, uint32_t data)
{
    if(address >= (PHYSICAL_MEMORY_WORDS))
    {
        printf("ERROR (write_word): direccion fuera de rango\n");
        return -1;
    }
    physical_memory[address] = data;
    return 0;
}

int is_kernel_address(uint32_t addr)
{
    return addr >= KERNEL_START && addr <= KERNEL_END;
}

uint32_t create_pageTable(uint32_t entry_count)
{
    if (kernel_pageTable_ind + entry_count > KERNEL_WORDS)
    {
        printf("ERROR (create_pageTable): no hay espacio para nuevas tablas de paginas.\n");
        return -1;
    }
    uint32_t base = kernel_pageTable_ind;
    kernel_pageTable_ind += entry_count;
    return base;
}

int allocate_page()
{
    for (int i=KERNEL_PAGES; i<NUM_PAGES; i++)
    {
        if (pages_bitmap[i] == 0)
        {
            pages_bitmap[i] = 1;
            //memset(&physical_memory[i*PAGE_NUM_WORDS], 0, PAGE_SIZE);
            return i;
        }
    }
    printf("ERROR (allocate_page): no hay paginas libres.\n");
    return -1;
}

//int load_segments(pcb, buffer, text_words, buffer + text_words, data_words)
int load_segments(struct my_PCB* pcb, uint32_t *buffer, size_t text_words, size_t data_words)
{
    size_t text_pages = (text_words + PAGE_NUM_WORDS - 1) / PAGE_NUM_WORDS;
    size_t data_pages = (data_words + PAGE_NUM_WORDS - 1) / PAGE_NUM_WORDS;
    
    //vectores que guardan los offset/numeros de paginas de los segmentos de text y data
    int *text_frames = malloc(text_pages * sizeof(int));
    int *data_frames = malloc(data_pages * sizeof(int));

    size_t i, copied = 0;  // contador de palabras copiadas para .text
    int offset;
    uint32_t base_index = 0; //la memoria de usuario empieza despues de la del kernel
    for (i=0; i<text_pages; i++)
    {
        offset = allocate_page();
        if (offset == -1)
        {
            free(text_frames);
            free(data_frames);
            return -1;
        }
        text_frames[i] = offset;

        base_index += offset * PAGE_NUM_WORDS; //calcular indice de pagina en funcion del offset
        
        size_t words_to_copy = PAGE_NUM_WORDS;
        if (copied + words_to_copy > text_words) words_to_copy = text_words - copied; //escritura de ultima pagina puede ser menor a PAGE_NUM_WORDS
        memcpy(&physical_memory[base_index], &buffer[copied], words_to_copy * sizeof(uint32_t));
        copied += words_to_copy;
    }
    copied = 0;
    for (i=0; i<data_pages; i++)
    {
        offset = allocate_page();
        if (offset == -1)
        {
            free(text_frames);
            free(data_frames);
            return -1;
        }
        data_frames[i] = offset;

        base_index += offset * PAGE_NUM_WORDS; //calcular indice de pagina en funcion del offset
        
        size_t words_to_copy = PAGE_NUM_WORDS;
        if (copied + words_to_copy > data_words) words_to_copy = data_words - copied; //escritura de ultima pagina puede ser menor a PAGE_NUM_WORDS
        memcpy(&physical_memory[base_index], &buffer[copied], words_to_copy * sizeof(uint32_t));
        copied += words_to_copy;
    }

    //crear la tabla de paginas en el kernel
    //TO-DO: crear tabla que guarde la direccion de cada pagina
    uint32_t pt_index = create_pageTable(2*text_pages + 2*data_pages); //por cada pagina una direccion virtual y otra fisica, de ahi el 2*
    if (pt_index == -1)
    {
        free(text_frames);
        free(data_frames);
        return -1;
    }
    /* //Esto no es correcto ya que puede haber mas de una pagina de texto y de datos
    physical_memory[pt_index + 0] = pcb->mm->code;
    physical_memory[pt_index + 1] = text_frames[0] * PAGE_SIZE;
    physical_memory[pt_index + 2] = pcb->mm->data;
    physical_memory[pt_index + 3] = data_frames[0] * PAGE_SIZE;
    */
    //actualizar PCB->mm
    pcb->mm->pgb = pt_index;

    free(text_frames);
    free(data_frames);
    return 0;
}

uint32_t translate_vaddr_to_paddr(struct my_PCB* pcb, uint32_t vaddr)
{
    //TO-DO
}