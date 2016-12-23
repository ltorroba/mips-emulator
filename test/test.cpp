#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"

#include "../src/Emulator.hpp"

TEST_CASE("Registers are retrieved properly", "[get_registers]") {
    Emulator* vm = new Emulator(128);

    SECTION("Retrieving register 0 will return 0") {
        REQUIRE(vm->get_register(0) == 0);
    }
}
