#include "../include/catch.hpp"
#include "../src/Utilities.hpp"

TEST_CASE("Test Branch instructions", "[step][Branch]") {
    Emulator* vm;

    SECTION("beq") {
        WORD program[1];
        program[0] = Utilities::I_instruction(0x04, 1, 2, 0b10); // beq r1, r2, 2(+8)
        program[1] = Utilities::R_instruction(0x00, 1, 0, 1, 0, 0x20); // add r1, r0, r1
        program[2] = Utilities::R_instruction(0x00, 1, 1, 2, 0, 0x20); // add r1, r1, r2

        SECTION("branches if registers are equal") {
            vm = new Emulator(128, program, 3);
            vm->set_register(1, 5);
            vm->set_register(2, 5);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 10);
        }

        SECTION("does not branch if registers are not equal") {
            vm = new Emulator(128, program, 3);
            vm->set_register(1, 5);
            vm->set_register(2, 8);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 5);
        }
    }
}
