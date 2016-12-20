#include <stdio.h>
#include <stdlib.h>

typedef unsigned int REGISTER;
typedef unsigned int ADDRESS;
typedef unsigned int WORD;

const int memory_size = 128;
unsigned char* memory;
REGISTER PC;

void memory_dump(int bytes_per_row) {
  // TODO: Assert arg is a multiple of 4
  int i;
  for(i = 0; i < memory_size / bytes_per_row; i++) {
    int j;
    for(j = 0; j < bytes_per_row; j++) {
      printf("0x%02x ", memory[i * bytes_per_row + j]);
    }
    printf("\n");
  }
}

int load_word(ADDRESS addr) {
  WORD temp = memory[addr];
  temp = temp | (memory[addr + 1] << 8);
  temp = temp | (memory[addr + 2] << 16);
  temp = temp | (memory[addr + 3] << 24);
  return temp;
}

void store_word(WORD word, ADDRESS addr) {
  memory[addr] = word;
  memory[addr + 1] = word >> 8;
  memory[addr + 2] = word >> 16;
  memory[addr + 3] = word >> 24;
}

// Executes the next instruction
// Returns: 0 if success, 1 if error
int step() {
  int opcode = load_word(PC) & (0b111111 << 26);
  return 0;
}

int main(void) {
  printf("Initializing memory...\n");
  memory = malloc(memory_size); // 1KB
  int i;
  for(i = 0; i < memory_size; i++) {
    memory[i] = 0;
  }

  printf("Initializing registers...\n");
  PC = 0;

  printf("Store word test: \n");
  store_word(15, 0);
  store_word(16, 4);
  store_word(255, 8);
  store_word(0xffffffff, 12);
  store_word(256, 16);

  printf("Load word test: 0x%02x (%u)\n", load_word(12), load_word(12));

  printf("Memory dump: \n");
  memory_dump(8);
  return 0;
}
