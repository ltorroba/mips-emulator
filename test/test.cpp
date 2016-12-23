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
