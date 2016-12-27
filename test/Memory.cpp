#include "../include/catch.hpp"
#include "../src/Utilities.hpp"

TEST_CASE("Test memory I/O instructions", "[step][Memory][I/O]") {
    Emulator* vm;

    SECTION("lb") {
        WORD program[5];
        program[0] = Utilities::I_instruction(20, 1, 5, 0); // lb r1, 0(r5)
        program[1] = Utilities::I_instruction(20, 2, 7, -3); // lb r2, -3(r7)
        program[2] = Utilities::I_instruction(20, 3, 5, 2); // lb r3, 2(r5)
        program[3] = Utilities::I_instruction(20, 4, 7, -1); // lb r4, -1(r7)
        program[4] = 0x0108ddcc;
        vm = new Emulator(128, program, 5);

        vm->set_register(5, 16);
        vm->set_register(7, 20);

        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);

        SECTION("functions as expected") {
            REQUIRE(vm->get_register(4) == 0x01);
            REQUIRE(vm->get_register(3) == 0x08);
        }

        SECTION("sign-extends loaded byte") {
            REQUIRE(vm->get_register(2) == 0xffffffdd);
            REQUIRE(vm->get_register(1) == 0xffffffcc);
        }
    }

    SECTION("lh") {
        WORD program[5];
        program[0] = Utilities::I_instruction(21, 1, 5, 0); // lh r1, 0(r5)
        program[1] = Utilities::I_instruction(21, 2, 7, -4); // lh r2, -4(r7)
        program[2] = Utilities::I_instruction(21, 3, 5, 2); // lh r3, 2(r5)
        program[3] = Utilities::I_instruction(21, 4, 7, -2); // lh r4, -2(r7)
        program[4] = Utilities::I_instruction(21, 6, 7, 1); // lh r6, 1(r7)
        program[5] = 0x0108ddcc;
        vm = new Emulator(128, program, 6);

        vm->set_register(5, 20);
        vm->set_register(7, 24);

        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);

        SECTION("functions as expected") {
            REQUIRE(vm->get_register(3) == 0x0108);
            REQUIRE(vm->get_register(4) == 0x0108);
        }

        SECTION("sign-extends loaded halfword") {
            REQUIRE(vm->get_register(1) == 0xffffddcc);
            REQUIRE(vm->get_register(2) == 0xffffddcc);
        }

        SECTION("traps on unaligned addresses (those that are not multiples of 2)") {
            REQUIRE(vm->step() == 1);
        }
    }
}
