#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <new>

#include "Emulator.hpp"

using namespace std;

void Emulator::init(size_t mem_size, WORD* program, size_t program_size) {
    memory_size = mem_size;

    memory = new BYTE[memory_size]();
    registers = new REGISTER[31]();
    PC = 0;

    // Load program to first portion of memory
    if(mem_size < program_size*4 - 1) {
        // TODO: Throw error
    }

    for(size_t i = 0; i < program_size; i++) {
        store_word(program[i], i*4);
    }
}

Emulator::Emulator(size_t mem_size) {
    init(mem_size, NULL, 0);
}

Emulator::Emulator(size_t mem_size, WORD* program, size_t program_size) {
    init(mem_size, program, program_size);
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
    int se_imm = ((instruction & 0x8000) != 0) ? (0xffff << 16) | imm : imm;

    // J-type (pseudo address is 26 bit, shifted by 2, and top 6 bits same as current PC)
    // 0x3FFFFFF is 26 lower bits set
    int pseudo_addr = (PC & (0b111111 << 26)) | ((instruction & 0x3FFFFFF) << 2);

    // Register values (unsigned)
    REGISTER Rs = get_register(rs);
    REGISTER Rt = get_register(rt);

    // Register values (signed)
    signed int Rss = *(REGISTER*)&Rs;
    signed int Rts = *(REGISTER*)&Rt;

    // Exception
    WORD exception = (rs << 15) | (rt << 10) | (rd << 5) | shamt;

    switch(opcode) {
        case 0: // ALU
            switch(func) {
                case 0: // sll
                    set_register(rd, Rt << shamt);
                    break;
                case 2: // srl
                    set_register(rd, Rt >> shamt);
                    break;
                case 3: // sra
                    set_register(rd, Rts >> shamt);
                    break;
                case 4: // sllv
                    set_register(rd, Rt << (Rs & 0b11111));
                    break;
                case 6: // srlv
                    set_register(rd, Rt >> (Rs & 0b11111));
                    break;
                case 7: // srav
                    set_register(rd, Rts >> (Rs & 0b11111));
                    break;
                case 8: // jr
                    PC = Rs - 4;
                    break;
                case 9: // jalr
                    set_register(31, PC + 4);
                    PC = Rs - 4;
                    break;
                case 10: // movz
                    if(get_register(rt) == 0)
                        set_register(rd, Rs);
                    break;
                case 11: // movn
                    if(get_register(rt) != 0)
                        set_register(rd, Rs);
                    break;
                case 13: // break
                    return exception;
                    break;
                case 16: // mfhi
                    set_register(rd, HI);
                    break;
                case 17: // mthi
                    HI = get_register(rs);
                    break;
                case 18: // mflo
                    set_register(rd, LO);
                    break;
                case 19: // mtlo
                    LO = get_register(rs);
                    break;
                case 24: // mult
                    {
                        int64_t result = (int64_t)Rss * (int64_t)Rts;
                        LO = result;
                        HI = result >> 32;
                    }
                    break;
                case 25: // multu
                    {
                        DWORD result = (DWORD)Rs * (DWORD)Rt;
                        LO = result;
                        HI = result >> 32;
                    }
                    break;
                case 26: // div
                    LO = Rss / Rts;
                    HI = Rss % Rts;
                    break;
                case 27: // divu
                    LO = Rs / Rt;
                    HI = Rs % Rt;
                    break;
                case 32: // add (traps on overflow)
                    if((Rss > 0 && Rts > 0 && (Rss + Rts) < 0) || (Rss < 0 && Rts < 0 && (Rss + Rts) > 0)) {
                        // Overflow occurred, trap
                        return 1;
                    }
                    set_register(rd, Rss + Rts);
                    break;
                case 33: // addu
                    set_register(rd, Rss + Rts);
                    break;
                case 34: // sub (traps on overflow)
                    if((Rss > 0 && Rts < 0 && (Rss - Rts) < 0) || (Rss < 0 && Rts > 0 && (Rss - Rts) > 0)) {
                        // Overflow occurred, trap
                        return 1;
                    }
                    set_register(rd, Rss - Rts);
                    break;
                case 35: // subu
                    set_register(rd, Rss - Rts);
                    break;
                case 36: // and
                    set_register(rd, Rs & Rt);
                    break;
                case 37: // or
                    set_register(rd, Rs | Rt);
                    break;
                case 38: // xor
                    set_register(rd, Rs ^ Rt);
                    break;
                case 39: // nor
                    set_register(rd, ~(Rs | Rt));
                    break;
                case 42: // slt
                    set_register(rd, Rss < Rts);
                    break;
                case 43: // sltu
                    set_register(rd, Rs < Rt);
                    break;
                case 48: // tge
                    if(Rss >= Rts)
                        return 1;
                    break;
                case 49: // tgeu
                    if(Rs >= Rt)
                        return 1;
                    break;
                case 50: // tlt
                    if(Rss < Rts)
                        return 1;
                    break;
                case 51: // tltu
                    if(Rs < Rt)
                        return 1;
                    break;
                case 52: // teq
                    if(Rs == Rt)
                        return 1;
                    break;
                case 54: // tne
                    if(Rs != Rt)
                        return 1;
                    break;
            }
            break;
        case 2: // j
            PC = pseudo_addr - 4;
            break;
        case 3: // jal
            set_register(31, PC + 4);
            PC = pseudo_addr - 4;
            break;
        case 4: // beq
            if(Rs == Rt)
                PC += (se_imm << 2) - 4;
            break;
        case 5: // bne
            if(Rs != Rt)
                PC += (se_imm << 2) - 4;
            break;
        case 6: // blez
            if(Rss <= 0)
                PC += (se_imm << 2) - 4;
            break;
        case 7: // bgtz
            if(Rss > 0)
                PC += (se_imm << 2) - 4;
            break;
        case 8: // addi (with overflow)
            if((Rss > 0 && se_imm > 0 && (Rss + se_imm) < 0) || (Rss < 0 && se_imm < 0 && (Rss + se_imm) > 0)) {
                return 1;
            }
            set_register(rt, Rss + se_imm);
            break;
        case 9: // addiu
            set_register(rt, Rss + se_imm);
            break;
        case 10: // slti
            set_register(rt, Rss < se_imm);
            break;
        case 11: // sltiu
            set_register(rt, Rss < imm);
            break;
        case 12: // andi
            set_register(rt, Rs & imm);
            break;
        case 13: // ori
            set_register(rt, Rs | imm);
            break;
        case 14: // xori
            set_register(rt, Rs ^ imm);
            break;
        case 15: // lui
            set_register(rt, (imm << 16) | (0xffff & Rs));
            break;
        case 32: // lb
            {
                BYTE res = load_byte(Rs + se_imm);
                REGISTER se_res = ((res & 0x80) != 0) ? (0xffffff << 8) | res : res;
                set_register(rt, se_res);
            }
            break;
        case 33: // lh
            if((Rs + se_imm) & 0x1)
                return 1; // Trap if not multiple of 2
            else
            {
                WORD a = load_byte(Rs + se_imm);
                WORD b = load_byte(Rs + se_imm + 1);
                WORD res = a | (b << 8);
                REGISTER se_res = ((res & 0x8000) != 0) ? (0xffff << 16) | res : res;
                set_register(rt, se_res);
            }
            break;
        case 34: // lwl
            {
                ADDRESS effective_address = Rs + se_imm;
                WORD aligned_word = load_word(effective_address & 0xfffffffc);
                int left_shift = 3 - (effective_address & 0b11);
                set_register(rt, aligned_word << (8 * left_shift));
            }
            break;
        case 35: // lw
            if((Rs + se_imm) & 0x3)
                return 1; // Trap if not multiple of 4
            else
            {
                set_register(rt, load_word(Rs + se_imm));
            }
            break;
        case 36: // lbu
            {
                BYTE res = load_byte(Rs + se_imm);
                set_register(rt, res);
            }
            break;
        case 37: // lhu
            if((Rs + se_imm) & 0x1)
                return 1; // Trap if not multiple of 2
            else
            {
                WORD a = load_byte(Rs + se_imm);
                WORD b = load_byte(Rs + se_imm + 1);
                WORD res = a | (b << 8);
                set_register(rt, res);
            }
            break;
        case 38: // lwr
            {
                ADDRESS effective_address = Rs + se_imm;
                WORD aligned_word = load_word(effective_address & 0xfffffffc);
                int right_shift = effective_address & 0b11;
                set_register(rt, aligned_word >> (8 * right_shift));
            }
            break;
    }

    PC = PC + 4;

    return 0;
}
