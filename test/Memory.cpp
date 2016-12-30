#include "../include/catch.hpp"
#include "../src/Utilities.hpp"

TEST_CASE("Test memory I/O instructions", "[step][Memory][I/O]") {
    Emulator* vm;

    SECTION("lb") {
        WORD program[5];
        program[0] = Utilities::I_instruction(32, 1, 5, 0); // lb r1, 0(r5)
        program[1] = Utilities::I_instruction(32, 2, 7, -3); // lb r2, -3(r7)
        program[2] = Utilities::I_instruction(32, 3, 5, 2); // lb r3, 2(r5)
        program[3] = Utilities::I_instruction(32, 4, 7, -1); // lb r4, -1(r7)
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
        program[0] = Utilities::I_instruction(33, 1, 5, 0); // lh r1, 0(r5)
        program[1] = Utilities::I_instruction(33, 2, 7, -4); // lh r2, -4(r7)
        program[2] = Utilities::I_instruction(33, 3, 5, 2); // lh r3, 2(r5)
        program[3] = Utilities::I_instruction(33, 4, 7, -2); // lh r4, -2(r7)
        program[4] = Utilities::I_instruction(33, 6, 7, 1); // lh r6, 1(r7)
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

    SECTION("lwl") {
        WORD program[5];
        program[0] = Utilities::I_instruction(34, 1, 5, 0); // lwl r1, 0(r5)
        program[1] = Utilities::I_instruction(34, 2, 5, 1); // lwl r2, 1(r5)
        program[2] = Utilities::I_instruction(34, 3, 5, 2); // lwl r3, 2(r5)
        program[3] = Utilities::I_instruction(34, 4, 5, 3); // lwl r4, 3(r5)
        program[4] = 0xaabbccdd;
        vm = new Emulator(128, program, 5);

        vm->set_register(5, 16);

        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);

        SECTION("functions as expected") {
            REQUIRE(vm->get_register(1) == 0xdd000000);
            REQUIRE(vm->get_register(2) == 0xccdd0000);
            REQUIRE(vm->get_register(3) == 0xbbccdd00);
            REQUIRE(vm->get_register(4) == 0xaabbccdd);
        }
    }

    SECTION("lw") {
        WORD program[5];
        program[0] = Utilities::I_instruction(35, 1, 5, 0); // lw r1, 0(r5)
        program[1] = Utilities::I_instruction(35, 2, 5, 1); // lw r4, 1(r5)
        program[2] = Utilities::I_instruction(35, 2, 5, 2); // lw r4, 2(r5)
        program[3] = Utilities::I_instruction(35, 2, 5, 3); // lw r4, 3(r5)
        program[4] = 0x01020304;
        vm = new Emulator(128, program, 5);

        vm->set_register(5, 16);

        REQUIRE(vm->step() == 0);

        SECTION("functions as expected") {
            REQUIRE(vm->get_register(1) == 0x01020304);
        }

        SECTION("traps on unaligned addresses (those that are not multiples of 4)") {
            REQUIRE(vm->step() == 1);
            REQUIRE(vm->step() == 1);
            REQUIRE(vm->step() == 1);
        }
    }

    SECTION("lbu") {
        WORD program[5];
        program[0] = Utilities::I_instruction(36, 1, 5, 0); // lbu r1, 0(r5)
        program[1] = Utilities::I_instruction(36, 2, 7, -3); // lbu r2, -3(r7)
        program[2] = Utilities::I_instruction(36, 3, 5, 2); // lbu r3, 2(r5)
        program[3] = Utilities::I_instruction(36, 4, 7, -1); // lbu r4, -1(r7)
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

        SECTION("does not sign-extend loaded byte") {
            REQUIRE(vm->get_register(2) == 0x000000dd);
            REQUIRE(vm->get_register(1) == 0x000000cc);
        }
    }

    SECTION("lhu") {
        WORD program[5];
        program[0] = Utilities::I_instruction(37, 1, 5, 0); // lhu r1, 0(r5)
        program[1] = Utilities::I_instruction(37, 2, 7, -4); // lhu r2, -4(r7)
        program[2] = Utilities::I_instruction(37, 3, 5, 2); // lhu r3, 2(r5)
        program[3] = Utilities::I_instruction(37, 4, 7, -2); // lhu r4, -2(r7)
        program[4] = Utilities::I_instruction(37, 6, 7, 1); // lhu r6, 1(r7)
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
            REQUIRE(vm->get_register(1) == 0x0000ddcc);
            REQUIRE(vm->get_register(2) == 0x0000ddcc);
        }

        SECTION("traps on unaligned addresses (those that are not multiples of 2)") {
            REQUIRE(vm->step() == 1);
        }
    }

    SECTION("lwr") {
        WORD program[5];
        program[0] = Utilities::I_instruction(38, 1, 5, 0); // lwr r1, 0(r5)
        program[1] = Utilities::I_instruction(38, 2, 5, 1); // lwr r2, 1(r5)
        program[2] = Utilities::I_instruction(38, 3, 5, 2); // lwr r3, 2(r5)
        program[3] = Utilities::I_instruction(38, 4, 5, 3); // lwr r4, 3(r5)
        program[4] = 0xaabbccdd;
        vm = new Emulator(128, program, 5);

        vm->set_register(5, 16);

        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);

        SECTION("functions as expected") {
            REQUIRE(vm->get_register(1) == 0xaabbccdd);
            REQUIRE(vm->get_register(2) == 0x00aabbcc);
            REQUIRE(vm->get_register(3) == 0x0000aabb);
            REQUIRE(vm->get_register(4) == 0x000000aa);
        }
    }

    SECTION("sb") {
        WORD program[5];
        program[0] = Utilities::I_instruction(40, 1, 5, 0); // sb r1, 0(r5)
        program[1] = Utilities::I_instruction(40, 2, 7, -3); // sb r2, -3(r7)
        program[2] = Utilities::I_instruction(40, 3, 5, 2); // sb r3, 2(r5)
        program[3] = Utilities::I_instruction(40, 4, 7, -1); // sb r4, -1(r7)
        program[4] = Utilities::I_instruction(35, 8, 5, 0); // lw r8, 0(r5)
        vm = new Emulator(128, program, 5);

        vm->set_register(1, 0x01);
        vm->set_register(2, 0x02);
        vm->set_register(3, 0x03);
        vm->set_register(4, 0x04);

        vm->set_register(5, 24);
        vm->set_register(7, 28);

        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);

        SECTION("functions as expected") {
            REQUIRE(vm->get_register(8) == 0x04030201);
        }
    }

    SECTION("sh") {
        WORD program[6];
        program[0] = Utilities::I_instruction(41, 8, 5, 0); // sh r1, 0(r5)
        program[1] = Utilities::I_instruction(32, 1, 5, 0); // lb r1, 0(r5)
        program[2] = Utilities::I_instruction(32, 2, 7, -3); // lb r2, -3(r7)
        program[3] = Utilities::I_instruction(33, 9, 5, 0); // lh r1, 1(r5)
        program[4] = Utilities::I_instruction(41, 8, 5, 1); // sh r1, 1(r5)
        vm = new Emulator(128, program, 5);

        vm->set_register(5, 32);
        vm->set_register(7, 36);
        vm->set_register(8, 0x00000102);

        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);

        SECTION("functions as expected") {
            REQUIRE(vm->get_register(1) == 0x02);
            REQUIRE(vm->get_register(2) == 0x01);
            REQUIRE(vm->get_register(9) == 0x0102);
        }

        SECTION("traps on unaligned addresses (those that are not multiples of 4)") {
            REQUIRE(vm->step() == 1);
        }
    }

    SECTION("sw") {
        WORD program[6];
        program[0] = Utilities::I_instruction(43, 8, 5, 0); // sw r1, 0(r5)
        program[1] = Utilities::I_instruction(32, 1, 5, 0); // lb r1, 0(r5)
        program[2] = Utilities::I_instruction(32, 2, 7, -3); // lb r2, -3(r7)
        program[3] = Utilities::I_instruction(32, 3, 5, 2); // lb r3, 2(r5)
        program[4] = Utilities::I_instruction(32, 4, 7, -1); // lb r4, -1(r7)
        program[5] = Utilities::I_instruction(43, 1, 5, 1); // sw r1, 0(r5)
        program[6] = Utilities::I_instruction(43, 1, 5, 2); // sw r1, 0(r5)
        program[7] = Utilities::I_instruction(43, 1, 5, 3); // sw r1, 0(r5)
        vm = new Emulator(128, program, 8);

        vm->set_register(5, 32);
        vm->set_register(7, 36);
        vm->set_register(8, 0x01020304);

        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);
        REQUIRE(vm->step() == 0);

        SECTION("functions as expected") {
            REQUIRE(vm->get_register(1) == 0x04);
            REQUIRE(vm->get_register(2) == 0x03);
            REQUIRE(vm->get_register(3) == 0x02);
            REQUIRE(vm->get_register(4) == 0x01);
            REQUIRE(vm->load_word(32) == 0x01020304);
        }

        SECTION("traps on unaligned addresses (those that are not multiples of 4)") {
            REQUIRE(vm->step() == 1);
            REQUIRE(vm->step() == 1);
            REQUIRE(vm->step() == 1);
        }
    }
}
