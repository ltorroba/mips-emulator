#include <cstdio>

typedef unsigned int REGISTER;
typedef unsigned int ADDRESS;
typedef unsigned int WORD;
typedef unsigned char BYTE;

class Emulator {
    BYTE* memory;
    REGISTER PC;
    REGISTER registers[31];
    size_t memory_size;

    public:
    Emulator(size_t mem_size);

    void dump_memory_range(BYTE* start, int length, int bytes_per_row);
    void memory_dump(int bytes_per_row);
    WORD load_word(ADDRESS addr);
    void store_word(WORD word, ADDRESS addr);
    WORD get_register(int number);
    void set_register(int number, WORD value);
    int step();
};
