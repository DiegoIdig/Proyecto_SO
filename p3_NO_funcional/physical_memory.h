#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "my_PCB.h"

// Tamaño de la memoria física en bytes -> 2^24 * 4 = 16777216 * 4 = 67108864 Bytes -> 67MB aprox
#define WORD_SIZE 4 //4 bytes por palabra
#define PAGE_SIZE 4096 //4KB por página

#define PHYSICAL_MEMORY_WORDS 16777216 //numero de palabras total de la memoria fisica (2^24 palabras)
#define PHYSICAL_MEMORY_SIZE (PHYSICAL_MEMORY_WORDS * WORD_SIZE) //en bytes

#define NUM_PAGES (PHYSICAL_MEMORY_SIZE / PAGE_SIZE) //numero de paginas o frames
#define PAGE_NUM_WORDS (PAGE_SIZE / WORD_SIZE) //1024 palabras por página

#define KERNEL_PAGES 1024
#define KERNEL_WORDS (KERNEL_PAGES * PAGE_NUM_WORDS)
#define KERNEL_MEMORY_SIZE (KERNEL_PAGES * PAGE_SIZE) //4096 * 1024 = 4194304 Bytes -> 4MB
#define KERNEL_START 0x000000
#define KERNEL_END 0x3FFFFF

#define USER_PAGES (NUM_PAGES - KERNEL_PAGES)
#define USER_WORDS (PHYSICAL_MEMORY_WORDS - KERNEL_WORDS)


//variables globales declaradas en el main
extern uint32_t *physical_memory;
extern uint32_t *pages_bitmap; //0: libre, 1: ocupado
//variable declarada en physical_memory.c
extern uint32_t kernel_pageTable_ind; //indice para crear la siguiente tabla de paginas del proceso

/*********************** FUNCIONES DECLARADAS ***********************/
uint32_t* create_physical_memory();
uint32_t* create_bitmap();
void free_physical_memory(uint32_t* physical_memory);
void free_bitmap(int* pages_bitmap);
uint32_t read_word(uint32_t address);
int write_word(uint32_t address, uint32_t data);
int is_kernel_address(uint32_t addr);
uint32_t create_pageTable(uint32_t entry_count);
int allocate_page();
int load_segments(struct my_PCB* pcb, uint32_t *buffer, size_t text_words, size_t data_words);
uint32_t translate_vaddr_to_paddr(struct my_PCB* pcb, uint32_t vaddr);

#endif
