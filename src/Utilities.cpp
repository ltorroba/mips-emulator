#include "Utilities.hpp"

// in assembly: OP rd, rs, rt
WORD Utilities::R_instruction(int opcode, int rd, int rs, int rt, int shamt, int func) {
    WORD instruction = 0x0;

    instruction |= (opcode & 0b111111) << 26;
    instruction |= (rs & 0b11111) << 21;
    instruction |= (rt & 0b11111) << 16;
    instruction |= (rd & 0b11111) << 11;
    instruction |= (shamt & 0b11111) << 6;
    instruction |= func & 0b111111;

    return instruction;
}

WORD Utilities::J_instruction(int opcode, int pseudo_addr) {
    WORD instruction = 0x0;

    instruction |= (opcode & 0b111111) << 26;
    instruction |= pseudo_addr & 0x3FFFFFF;

    return instruction;
}
