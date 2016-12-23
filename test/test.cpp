#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"

#include "../src/Emulator.hpp"

TEST_CASE("Emulator is constructed properly", "[Emulator][get_register]") {
    Emulator* vm = new Emulator(128);

    SECTION("Retrieving any register will return 0") {
        for(int i = 0; i < 32; i++) {
            REQUIRE(vm->get_register(i) == 0);
        }
    }

    SECTION("Memory will be intialized to 0") {
        for(int i = 0; i < 128; i++) {
            REQUIRE(vm->load_byte(i) == 0);
        }
    }
}

TEST_CASE("Register get & set are functioning properly", "[get_register][set_register]") {
    Emulator* vm = new Emulator(128);

    SECTION("Register 0 is always 0") {
        REQUIRE(vm->get_register(0) == 0);
        vm->set_register(0, 1);
        REQUIRE(vm->get_register(0) == 0);
    }

    SECTION("Registers 1-31 can be set to any value") {
        for(int i = 1; i < 32; i++) {
            vm->set_register(i, 5);
            REQUIRE(vm->get_register(i) == 5);
        }
    }
}

TEST_CASE("Memory operations are functioning properly", "[load_byte][store_byte][load_word][store_word]") {
    Emulator* vm = new Emulator(128);

    SECTION("Can individually read and write bytes on any part of the memory") {
        for(int i = 1; i < 128; i++) {
            vm->store_byte(0xff, i);
            REQUIRE(vm->load_byte(i) == 0xff);
        }
    }

    SECTION("Write words as four bytes") {
        vm->store_word(0xffffffff, 0);
        REQUIRE(vm->load_byte(0) == 0xff);
        REQUIRE(vm->load_byte(1) == 0xff);
        REQUIRE(vm->load_byte(2) == 0xff);
        REQUIRE(vm->load_byte(3) == 0xff);
    }

    SECTION("Load word from four bytes") {
        vm->store_word(0xffffffff, 0);
        REQUIRE(vm->load_word(0) == 0xffffffff);
    }

    SECTION("Write words in Little-Endian") {
        vm->store_word(0x01020304, 0);
        REQUIRE(vm->load_byte(0) == 0x04);
        REQUIRE(vm->load_byte(1) == 0x03);
        REQUIRE(vm->load_byte(2) == 0x02);
        REQUIRE(vm->load_byte(3) == 0x01);
    }

    SECTION("Read words in Little-Endian") {
        vm->store_word(0x01020304, 0);
        REQUIRE(vm->load_word(0) == 0x01020304);
    }
}
