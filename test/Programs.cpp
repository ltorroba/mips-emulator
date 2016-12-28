#include "../include/catch.hpp"
#include "../src/Utilities.hpp"

TEST_CASE("Test system holistically", "[step][system-tests]") {
    Emulator* vm;

    SECTION("Simple memory I/O") {
        WORD program[7];
        program[0] = 0x3c01aabb; // lui $1, 0xffffaabb
        program[1] = 0x3429ccdd; // ori $9, $1, 0x0000ccdd
        program[2] = 0xafa90004; // sw $9, 0x00000004($29)
        program[3] = 0x23bd0004; // addi $29, $29, 0x00000004
        program[4] = 0x8baa0001; // lwl $10, 0x00000001($29)
        program[5] = 0x9bab0001; // lwr $11, 0x00000001($29)
        program[6] = 0x83ac0000; // lb $12, 0x00000000($29)
        vm = new Emulator(128, program, 7);

        // set $29 (stack-pointer) to high memory address
        vm->set_register(29, 32);

        // Execute program
        for(int i = 0; i < 7; i++) {
            REQUIRE(vm->step() == 0);
        }

        // Check side-effects
        REQUIRE(vm->get_register(9) == 0xaabbccdd);
        REQUIRE(vm->get_register(10) == 0xccdd0000);
        REQUIRE(vm->get_register(11) == 0x00aabbcc);
        REQUIRE(vm->get_register(12) == 0xffffffdd);
    }
}
