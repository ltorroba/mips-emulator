#include "Emulator.hpp"

class Utilities {
    public:
    static WORD R_instruction(int opcode, int rd, int rs, int rt, int shamt, int func);
};
