#include <stdio.h>
#include <iostream>

#include "Emulator.hpp"

using namespace std;

int main(int argc, char * argv[]) {
    Emulator* vm = new Emulator(128);

    cout << "Store word test:" << endl;
    vm->store_word(15, 0);
    vm->store_word(16, 4);
    vm->store_word(255, 8);
    vm->store_word(0xffffffff, 12);
    vm->store_word(256, 16);

    printf("Load word test: 0x%02x (%u)\n", vm->load_word(12), vm->load_word(12));

    cout << "Memory dump:" << endl;
    vm->memory_dump(8);
    return 0;
}
