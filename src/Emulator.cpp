#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <new>

#include "Emulator.hpp"

using namespace std;

Emulator::Emulator(size_t mem_size) {
    memory_size = mem_size;

    // TODO: Improve this (use memset?)
    memory = new BYTE[memory_size];
    for(size_t i = 0; i < memory_size; i++) {
        memory[i] = 0;
    }

    // TODO: Improve, use memset?
    for(size_t i = 0; i < 31; i++) {
        registers[i] = 0;
    }

    PC = 0;
}

void Emulator::dump_memory_range(BYTE* start, int length, int bytes_per_row) {
    // TODO: bytes_per_row is a multiple of 4
    for(int i = 0; i < length / bytes_per_row; i++) {
        int j;
        for(j = 0; j < bytes_per_row; j++) {
            printf("0x%02x ", *(start + i * bytes_per_row + j));
        }
        printf("\n");
    }
}

void Emulator::memory_dump(int bytes_per_row) {
    dump_memory_range(memory, memory_size, bytes_per_row);
}

WORD Emulator::load_word(ADDRESS addr) {
    WORD temp = memory[addr];
    temp = temp | (memory[addr + 1] << 8);
    temp = temp | (memory[addr + 2] << 16);
    temp = temp | (memory[addr + 3] << 24);
    return temp;
}

void Emulator::store_word(WORD word, ADDRESS addr) {
    memory[addr] = word;
    memory[addr + 1] = word >> 8;
    memory[addr + 2] = word >> 16;
    memory[addr + 3] = word >> 24;
}

BYTE Emulator::load_byte(ADDRESS addr) {
    return memory[addr];
}

void Emulator::store_byte(BYTE byte, ADDRESS addr) {
    memory[addr] = byte;
}

WORD Emulator::get_register(int number) {
    if(number == 0) return 0;
    else return registers[number - 1];
}

void Emulator::set_register(int number, WORD value) {
    if(number > 0)
        registers[number - 1] = value;
}

// Executes the next instruction
// Returns: 0 if success, 1 if error
int Emulator::step() {
    WORD instruction = load_word(PC);
    int opcode = (instruction >> 26) & 0b111111;

    // R-type instruction
    int rs = (instruction >> 21) & 0b11111;
    int rt = (instruction >> 16) & 0b11111;
    int rd = (instruction >> 11) & 0b11111;
    int shamt = (instruction >> 6) & 0b11111;
    int func = instruction & 0b111111;

    // I-type instruction (rs, rt as above)
    int imm = instruction & 65535; // 16 lower bits

    // J-type (pseudo address is 26 bit, shifted by 2, and top 6 bits same as current PC)
    int pseudo_addr = (PC & (0b111111 << 26)) | (instruction & 0x3FFFFFF); // 0x3FFFFFF is 26 lower bits set

    // Register values (unsigned)
    REGISTER Rs = get_register(rs);
    REGISTER Rt = get_register(rt);

    // Register values (signed)
    signed int Rss = *(REGISTER*)&Rs;
    signed int Rts = *(REGISTER*)&Rt;

    switch(opcode) {
        case 0b000000: // ALU
            switch(func) {
                case 0b100000: // add (traps on overflow)
                    if((Rss > 0 && Rts > 0 && (Rss + Rts) < 0) | (Rss < 0 && Rts < 0 && (Rss + Rts) > 0)) {
                        // Overflow occurred, trap
                        return 1;
                    }
                    set_register(rd, Rss + Rts);
                    break;
                case 0b100001: // addu
                    set_register(rd, Rss + Rts);
                    break;
            }
            break;
    }

    PC = PC + 4;

    return 0;
}
