#include "../include/catch.hpp"
#include "../src/Utilities.hpp"

TEST_CASE("Test I-Type instructions", "[step][I-type]") {
    Emulator* vm;

    SECTION("addi") {
        WORD program[1];
        program[0] = Utilities::I_instruction(8, 1, 2, 30); // addi r1, r2, 30
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected with positive, normal input") {
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 33);
        }

        SECTION("functions as expected with negative, normal input") {
            vm->set_register(2, -33);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == -3);
        }

        SECTION("traps on overflow") {
            vm->set_register(2, 0x7FFFFFFF);

            REQUIRE(vm->step() == 1);
        }

        SECTION("doesn't trap on overflow bound") {
            vm->set_register(2, 0x7FFFFFE1);

            REQUIRE(vm->step() == 0);
        }

        SECTION("traps on underflow") {
            program[0] = Utilities::I_instruction(8, 1, 2, -1); // addi r1, r2, -1
            vm = new Emulator(128, program, 1);
            vm->set_register(2, 0x80000000);

            REQUIRE(vm->step() == 1);
        }

        SECTION("doesn't trap on underflow bound") {
            program[0] = Utilities::I_instruction(8, 1, 2, 0); // addi r1, r2, 0
            vm = new Emulator(128, program, 1);
            vm->set_register(2, 0x80000000);

            REQUIRE(vm->step() == 0);
        }
    }

    SECTION("addiu") {
        WORD program[1];
        program[0] = Utilities::I_instruction(9, 1, 2, 30); // addiu r1, r2, 30
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected with positive, normal input") {
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 33);
        }

        SECTION("functions as expected with negative, normal input") {
            vm->set_register(2, -33);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == -3);
        }
    }

    SECTION("slti") {
        WORD program[1];
        program[0] = Utilities::I_instruction(10, 2, 1, 30); // slti r2, r1, 30
        vm = new Emulator(128, program, 1);

        SECTION("sets when appropriate (2 +ve values)") {
            vm->set_register(1, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 1);
        }

        SECTION("doesn't set when appropriate (2 +ve values)") {
            vm->set_register(2, 1);
            vm->set_register(1, 33);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0);
        }

        SECTION("interprets signed values correctly") {
            vm->set_register(1, 0xffffffff);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 1);
        }

        SECTION("immediate is sign-extended") {
            program[0] = Utilities::I_instruction(10, 2, 1, 0xffff); // slti r1, -1(-4)

            vm = new Emulator(128, program, 1);
            vm->set_register(1, -2000);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 1);
        }
    }

    SECTION("sltiu") {
        WORD program[1];
        program[0] = Utilities::I_instruction(10, 2, 1, 30); // sltiu r2, r1, 30
        vm = new Emulator(128, program, 1);

        SECTION("sets when appropriate (2 +ve values)") {
            vm->set_register(1, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 1);
        }

        SECTION("doesn't set when appropriate (2 +ve values)") {
            vm->set_register(2, 1);
            vm->set_register(1, 33);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0);
        }

        SECTION("immediate is zero-extended") {
            program[0] = Utilities::I_instruction(10, 2, 1, 0xffff); // slti r1, -1(-4)

            vm = new Emulator(128, program, 1);
            vm->set_register(2, 1);
            vm->set_register(1, 0xffffffff);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0);
        }
    }

    SECTION("andi") {
        WORD program[1];
        program[0] = Utilities::I_instruction(12, 2, 1, 0xaaaa); // and r3, r1, r2
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected") {
            vm->set_register(1, 0xcccccccc);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0x00008888);
        }
    }
}
