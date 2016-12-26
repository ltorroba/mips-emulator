#include "../include/catch.hpp"
#include "../src/Utilities.hpp"

TEST_CASE("Test ALU operations", "[step][ALU]") {
    Emulator* vm;

    SECTION("sll") {
        WORD program[1];

        SECTION("functions as identity/NOP when shift is 0") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 0, 0x00); // sll r2, r1, 0
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b1111);
        }

        SECTION("functions as expected") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 5, 0x00); // sll r2, r1, 5
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b111100000);
        }
    }

    SECTION("srl") {
        WORD program[1];

        SECTION("functions as identity/NOP when shift is 0") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 0, 0x02); // srl r2, r1, 0
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b1111);
        }

        SECTION("functions as expected") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 2, 0x02); // srl r2, r1, 2
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b11);
        }

        SECTION("does not sign-extend") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 4, 0x02); // srl r2, r1, 2
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0xffffffff);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0x0fffffff);
        }
    }

    SECTION("sra") {
        WORD program[1];

        SECTION("functions as identity/NOP when shift is 0") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 0, 0x03); // sra r2, r1, 0
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b1111);
        }

        SECTION("functions as expected") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 2, 0x03); // sra r2, r1, 2
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b11);
        }

        SECTION("does sign-extend") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 4, 0x03); // sra r2, r1, 2
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0xffffffff);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0xffffffff);
        }
    }

    SECTION("sllv") {
        WORD program[1];

        SECTION("functions as identity/NOP when register value is 0") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 0, 0x04); // sllv r2, r1, r0
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b1111);
        }

        SECTION("functions as expected") {
            program[0] = Utilities::R_instruction(0x00, 2, 3, 1, 0, 0x04); // sllv r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);
            vm->set_register(3, 2);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b111100);
        }

        SECTION("uses only lowest 5 least-significant register bits") {
            program[0] = Utilities::R_instruction(0x00, 2, 3, 1, 0, 0x04); // sllv r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0xffffffff);
            vm->set_register(3, 0b100100); // should interpret as 4 and not 68

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0xfffffff0);
        }
    }

    SECTION("srlv") {
        WORD program[1];

        SECTION("functions as identity/NOP when shift is 0") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 0, 0x06); // srlv r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b1111);
        }

        SECTION("functions as expected") {
            program[0] = Utilities::R_instruction(0x00, 2, 3, 1, 0, 0x06); // srlv r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);
            vm->set_register(3, 2);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b11);
        }

        SECTION("does not sign-extend") {
            program[0] = Utilities::R_instruction(0x00, 2, 3, 1, 0, 0x06); // srlv r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0xffffffff);
            vm->set_register(3, 4);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0x0fffffff);
        }

        SECTION("uses only lowest 5 least-significant register bits") {
            program[0] = Utilities::R_instruction(0x00, 2, 3, 1, 0, 0x06); // srlv r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0xffffffff);
            vm->set_register(3, 0b100100); // should interpret as 4 and not 68

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0x0fffffff);
        }
    }

    SECTION("srav") {
        WORD program[1];

        SECTION("functions as identity/NOP when shift is 0") {
            program[0] = Utilities::R_instruction(0x00, 2, 0, 1, 0, 0x07); // srav r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b1111);
        }

        SECTION("functions as expected") {
            program[0] = Utilities::R_instruction(0x00, 2, 3, 1, 0, 0x07); // srav r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0b1111);
            vm->set_register(3, 2);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0b11);
        }

        SECTION("does sign-extend") {
            program[0] = Utilities::R_instruction(0x00, 2, 3, 1, 0, 0x07); // srav r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0xffffffff);
            vm->set_register(3, 4);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0xffffffff);
        }

        SECTION("uses only lowest 5 least-significant register bits") {
            program[0] = Utilities::R_instruction(0x00, 2, 3, 1, 0, 0x07); // srav r2, r1, r3
            vm = new Emulator(128, program, 1);
            vm->set_register(1, 0xf0ffffff);
            vm->set_register(3, 0b100100); // should interpret as 4 and not 68

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 0xff0fffff);
        }
    }

    SECTION("jr") {
        WORD program[3];
        program[0] = Utilities::R_instruction(0x00, 0, 1, 0, 0, 0x08); // jr r1
        program[1] = Utilities::R_instruction(0x00, 1, 1, 2, 0, 0x20); // add r1, r1, r2
        program[2] = Utilities::R_instruction(0x00, 1, 1, 3, 0, 0x20); // add r1, r1, r3

        vm = new Emulator(128, program, 3);
        vm->set_register(1, 8);
        vm->set_register(2, 5);
        vm->set_register(3, 10);

        SECTION("functions as expected") {
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 18);
        }
    }

    SECTION("jalr") {
        WORD program[3];
        program[0] = Utilities::R_instruction(0x00, 0, 1, 0, 0, 0x09); // jalr r1
        program[1] = Utilities::R_instruction(0x00, 1, 1, 2, 0, 0x20); // add r1, r1, r2
        program[2] = Utilities::R_instruction(0x00, 1, 1, 3, 0, 0x20); // add r1, r1, r3

        vm = new Emulator(128, program, 3);
        vm->set_register(1, 8);
        vm->set_register(2, 5);
        vm->set_register(3, 10);

        SECTION("functions as expected") {
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 18);
            REQUIRE(vm->get_register(31) == 4);
        }
    }

    SECTION("movz") {
        WORD program[1];

        SECTION("executes when condition is zero") {
            program[0] = Utilities::R_instruction(0x00, 1, 2, 0, 0, 0x0a); // movz r1, r2, r0
            vm = new Emulator(128, program, 1);
            vm->set_register(2, 10);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 10);
        }

        SECTION("does not execute when condition is non-zero") {
            program[0] = Utilities::R_instruction(0x00, 1, 2, 2, 0, 0x0a); // movz r1, r2, r2
            vm = new Emulator(128, program, 1);
            vm->set_register(2, 10);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 0);
        }
    }

    SECTION("movn") {
        WORD program[1];

        SECTION("does not execute when condition is zero") {
            program[0] = Utilities::R_instruction(0x00, 1, 2, 0, 0, 0x0b); // movn r1, r2, r0
            vm = new Emulator(128, program, 1);
            vm->set_register(2, 10);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 0);
        }

        SECTION("executes when condition is non-zero") {
            program[0] = Utilities::R_instruction(0x00, 1, 2, 2, 0, 0x0b); // movn r1, r2, r2
            vm = new Emulator(128, program, 1);
            vm->set_register(2, 10);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(1) == 10);
        }
    }

    SECTION("break") {
        WORD program[1];
        program[0] = Utilities::R_instruction(0x00, 0, 0b10000, 0, 0b10010, 0x0d); // break 0x80012
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected") {
            REQUIRE(vm->step() == 0b10000000000000010010);
        }
    }

    SECTION("mfhi and mthi") {
        WORD program[2];
        program[0] = Utilities::R_instruction(0x00, 0, 1, 0, 0, 17); // mthi r1
        program[1] = Utilities::R_instruction(0x00, 2, 0, 0, 0, 16); // mfhi r2
        vm = new Emulator(128, program, 2);
        vm->set_register(1, 10);

        SECTION("function as expected") {
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 10);
        }
    }

    SECTION("mflo and mtlo") {
        WORD program[2];
        program[0] = Utilities::R_instruction(0x00, 0, 1, 0, 0, 19); // mtlo r1
        program[1] = Utilities::R_instruction(0x00, 2, 0, 0, 0, 18); // mflo r2
        vm = new Emulator(128, program, 2);
        vm->set_register(1, 10);

        SECTION("function as expected") {
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(2) == 10);
        }
    }

    SECTION("mult") {
        WORD program[3];
        program[0] = Utilities::R_instruction(0x00, 0, 1, 2, 0, 24); // mult r1, r2
        program[1] = Utilities::R_instruction(0x00, 3, 0, 0, 0, 16); // mfhi r3
        program[2] = Utilities::R_instruction(0x00, 4, 0, 0, 0, 18); // mflo r4

        SECTION("two positive numbers multiplication works correctly") {
            vm = new Emulator(128, program, 3);
            vm->set_register(1, 0x7fffffff);
            vm->set_register(2, 32);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);

            SECTION("high bits set correctly") {
                REQUIRE(vm->get_register(3) == 0b1111);
            }

            SECTION("low bits set correctly") {
                REQUIRE(vm->get_register(4) == 0xffffffe0);
            }
        }

        SECTION("mixed numbers multiplication works correctly") {
            vm = new Emulator(128, program, 3);
            vm->set_register(1, 0x7fffffff);
            vm->set_register(2, 0xfffffff0); // -16

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);

            SECTION("high bits set correctly") {
                REQUIRE(vm->get_register(3) == 0xfffffff8);
            }

            SECTION("low bits set correctly") {
                REQUIRE(vm->get_register(4) == 0x00000010);
            }
        }

        SECTION("negative numbers multiplication works correctly") {
            vm = new Emulator(128, program, 3);
            vm->set_register(1, 0xffffffff); // -1
            vm->set_register(2, 0xfffffff0); // -16

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);

            SECTION("high bits set correctly") {
                REQUIRE(vm->get_register(3) == 0x0);
            }

            SECTION("low bits set correctly") {
                REQUIRE(vm->get_register(4) == 0x10);
            }
        }
    }

    SECTION("multu") {
        WORD program[3];
        program[0] = Utilities::R_instruction(0x00, 0, 1, 2, 0, 25); // mult r1, r2
        program[1] = Utilities::R_instruction(0x00, 3, 0, 0, 0, 16); // mfhi r3
        program[2] = Utilities::R_instruction(0x00, 4, 0, 0, 0, 18); // mflo r4

        SECTION("two positive numbers multiplication works correctly") {
            vm = new Emulator(128, program, 3);
            vm->set_register(1, 0x7fffffff);
            vm->set_register(2, 32);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);

            SECTION("high bits set correctly") {
                REQUIRE(vm->get_register(3) == 0b1111);
            }

            SECTION("low bits set correctly") {
                REQUIRE(vm->get_register(4) == 0xffffffe0);
            }
        }

        SECTION("would-be negative number multiplication works correctly") {
            vm = new Emulator(128, program, 3);
            vm->set_register(1, 0x7fffffff);
            vm->set_register(2, 0xfffffff0); // -16

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);
            REQUIRE(vm->step() == 0);

            SECTION("high bits set correctly") {
                REQUIRE(vm->get_register(3) == 0x7ffffff7);
            }

            SECTION("low bits set correctly") {
                REQUIRE(vm->get_register(4) == 0x00000010);
            }
        }
    }

    SECTION("div") {
        WORD program[3];
        program[0] = Utilities::R_instruction(0x00, 0, 1, 2, 0, 26); // div r1, r2
        program[1] = Utilities::R_instruction(0x00, 3, 0, 0, 0, 16); // mfhi r3
        program[2] = Utilities::R_instruction(0x00, 4, 0, 0, 0, 18); // mflo r4

        SECTION("divide two positive numbers") {
            SECTION("no remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, 16);
                vm->set_register(2, 4);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 0);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE(vm->get_register(4) == 4);
                }
            }

            SECTION("with remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, 16);
                vm->set_register(2, 3);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 1);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE(vm->get_register(4) == 5);
                }
            }
        }

        SECTION("divide positive by negative number") {
            SECTION("no remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, 16);
                vm->set_register(2, -4);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 0);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE((signed)vm->get_register(4) == -4);
                }
            }

            SECTION("with remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, 16);
                vm->set_register(2, -3);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 1);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE((signed)vm->get_register(4) == -5);
                }
            }
        }

        SECTION("divide negative by positive number") {
            SECTION("no remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, -16);
                vm->set_register(2, 4);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 0);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE((signed)vm->get_register(4) == -4);
                }
            }

            SECTION("with remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, -16);
                vm->set_register(2, 3);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE((signed)vm->get_register(3) == -1);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE((signed)vm->get_register(4) == -5);
                }
            }
        }

        SECTION("divide two negative numbers") {
            SECTION("no remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, -16);
                vm->set_register(2, -4);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 0);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE(vm->get_register(4) == 4);
                }
            }

            SECTION("with remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, -16);
                vm->set_register(2, -3);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE((signed)vm->get_register(3) == -1);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE(vm->get_register(4) == 5);
                }
            }
        }
    }

    SECTION("divu") {
        WORD program[3];
        program[0] = Utilities::R_instruction(0x00, 0, 1, 2, 0, 27); // divu r1, r2
        program[1] = Utilities::R_instruction(0x00, 3, 0, 0, 0, 16); // mfhi r3
        program[2] = Utilities::R_instruction(0x00, 4, 0, 0, 0, 18); // mflo r4

        SECTION("divide two positive numbers") {
            SECTION("no remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, 16);
                vm->set_register(2, 4);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 0);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE(vm->get_register(4) == 4);
                }
            }

            SECTION("with remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, 16);
                vm->set_register(2, 3);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 1);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE(vm->get_register(4) == 5);
                }
            }
        }

        SECTION("divide would-be negative by positive number") {
            SECTION("no remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, 0xfffffff0);
                vm->set_register(2, 4);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 0);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE(vm->get_register(4) == 1073741820);
                }
            }

            SECTION("with remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, 0xfffffff0);
                vm->set_register(2, 3);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 0);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE(vm->get_register(4) == 1431655760);
                }
            }
        }

        SECTION("divide two would-be negative numbers") {
            SECTION("with remainder") {
                vm = new Emulator(128, program, 3);
                vm->set_register(1, 0xfffffff0);
                vm->set_register(2, 0xfffffffc);

                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);
                REQUIRE(vm->step() == 0);

                SECTION("high bits (remainder) set correctly") {
                    REQUIRE(vm->get_register(3) == 0xfffffff0);
                }

                SECTION("low bits (quotient) set correctly") {
                    REQUIRE(vm->get_register(4) == 0);
                }
            }
        }
    }

    SECTION("add") {
        WORD program[1];
        program[0] = Utilities::R_instruction(0x00, 3, 1, 2, 0, 0x20); // add r3, r1, r2
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected with positive, normal input") {
            vm->set_register(1, 2);
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 5);
        }

        SECTION("functions as expected with negative, normal input") {
            vm->set_register(1, -2);
            vm->set_register(2, -3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == -5);
        }

        SECTION("functions as expected with mixed, normal input") {
            vm->set_register(1, -2);
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 1);
        }

        SECTION("traps on overflow") {
            vm->set_register(1, 0x7FFFFFFF);
            vm->set_register(2, 1);

            REQUIRE(vm->step() == 1);
        }

        SECTION("doesn't trap on overflow bound") {
            vm->set_register(1, 0x7FFFFFFF);
            vm->set_register(2, 0);

            REQUIRE(vm->step() == 0);
        }

        SECTION("traps on underflow") {
            vm->set_register(1, 0x80000000);
            vm->set_register(2, -1);

            REQUIRE(vm->step() == 1);
        }

        SECTION("doesn't trap on underflow bound") {
            vm->set_register(1, 0x80000000);
            vm->set_register(2, 0);

            REQUIRE(vm->step() == 0);
        }
    }

    SECTION("addu") {
        WORD program[1];
        program[0] = Utilities::R_instruction(0x00, 3, 1, 2, 0, 0x21); // addu r3, r1, r2
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected with positive, normal input") {
            vm->set_register(1, 2);
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 5);
        }

        SECTION("functions as expected with negative, normal input") {
            vm->set_register(1, -2);
            vm->set_register(2, -3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == -5);
        }

        SECTION("functions as expected with mixed, normal input") {
            vm->set_register(1, -2);
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 1);
        }

        SECTION("doesn't trap on overflow") {
            vm->set_register(1, 0x7FFFFFFF);
            vm->set_register(2, 1);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 0x80000000);
        }

        SECTION("doesn't trap on underflow") {
            vm->set_register(1, 0x80000000);
            vm->set_register(2, -1);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 0x7FFFFFFF);
        }
    }

    SECTION("sub") {
        WORD program[1];
        program[0] = Utilities::R_instruction(0x00, 3, 1, 2, 0, 34); // sub r3, r1, r2
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected with positive, normal input") {
            vm->set_register(1, 2);
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == -1);
        }

        SECTION("functions as expected with negative, normal input") {
            vm->set_register(1, -2);
            vm->set_register(2, -3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 1);
        }

        SECTION("functions as expected with mixed, normal input") {
            vm->set_register(1, -2);
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == -5);
        }

        SECTION("traps on overflow") {
            vm->set_register(1, 0x7FFFFFFF);
            vm->set_register(2, -1);

            REQUIRE(vm->step() == 1);
        }

        SECTION("doesn't trap on overflow bound") {
            vm->set_register(1, 0x7FFFFFFF);
            vm->set_register(2, 0);

            REQUIRE(vm->step() == 0);
        }

        SECTION("traps on underflow") {
            vm->set_register(1, 0x80000000);
            vm->set_register(2, 1);

            REQUIRE(vm->step() == 1);
        }

        SECTION("doesn't trap on underflow bound") {
            vm->set_register(1, 0x80000000);
            vm->set_register(2, 0);

            REQUIRE(vm->step() == 0);
        }
    }

    SECTION("subu") {
        WORD program[1];
        program[0] = Utilities::R_instruction(0x00, 3, 1, 2, 0, 35); // subu r3, r1, r2
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected with positive, normal input") {
            vm->set_register(1, 2);
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == -1);
        }

        SECTION("functions as expected with negative, normal input") {
            vm->set_register(1, -2);
            vm->set_register(2, -3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 1);
        }

        SECTION("functions as expected with mixed, normal input") {
            vm->set_register(1, -2);
            vm->set_register(2, 3);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == -5);
        }

        SECTION("doesn't trap on overflow") {
            vm->set_register(1, 0x7FFFFFFF);
            vm->set_register(2, -1);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 0x80000000);
        }

        SECTION("doesn't trap on underflow") {
            vm->set_register(1, 0x80000000);
            vm->set_register(2, 1);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 0x7FFFFFFF);
        }
    }

    SECTION("and") {
        WORD program[1];
        program[0] = Utilities::R_instruction(0x00, 3, 1, 2, 0, 36); // and r3, r1, r2
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected") {
            vm->set_register(1, 0b10011);
            vm->set_register(2, 0b10101);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 0b10001);
        }
    }

    SECTION("or") {
        WORD program[1];
        program[0] = Utilities::R_instruction(0x00, 3, 1, 2, 0, 37); // or r3, r1, r2
        vm = new Emulator(128, program, 1);

        SECTION("functions as expected") {
            vm->set_register(1, 0b10011);
            vm->set_register(2, 0b10101);

            REQUIRE(vm->step() == 0);
            REQUIRE(vm->get_register(3) == 0b10111);
        }
    }
}
