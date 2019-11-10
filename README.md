# mips-emulator

A simple MIPS instruction set emulator. I had recently learnt a bit about the MIPS instruction set architecture
during one of my courses and decided to try to use C to create a simple emulator using test-driven design.
As such, my workflow consisted of picking an instruction, writing a test for it, and then writing the code for it.

Broadly speaking, the `Emulator` class allows instancing a memory region, where both code and data are stored
(as this was mostly a proof of concept, I decided not to worry about the obvious security flaws in this approach).
The `step()` function is where the code is read (based on the current program counter `PC` value), executed, and
memory is mutated.

The `Utilities` class allows for a "simpler" coding experience: as long as you know the type of instruction you
want to execute, its opcode and the parameter values involved, you can call one of those functions to generate the
word encoding it.
At the time I looked into using an existing C/C++ to MIPS compiler to make this even simpler (and have more complex
tests). In particular I thought of using the ability to embed direct assembly into C/C++ code to allow calling
instructions specific to my implementation (e.g. reading from a virtual sensor, and being able to actuate a motor
in a "virtual world").
However, as binaries contain a lot more than just code, this ended up being more time consuming than I thought.

The bulk of the code, and examples of how the project works, can be found
in the tests directory. In essence, you create an `Emulator`, upload
a program, and manually set the memory/registers to the values you want.

# Running and building

These instructions apply to MacOS, but should work in other environments. The easiest way to get started is to
clone the repo and run `./build_tests.sh`. Alternatively:

1. Clone the repository and `cd` into it
2. To build, run `make`
3. To build the tests, run `make tests`

The `bin` directory will contain:
- `Emulator`: This executable corresponds to the `main` file. It tests some simple functionality (this is 
great for debugging during development).
- `tests`: This runs all the unit tests.

# Future work

- I later took a class (Compiling Techniques) where I created a C-to-MIPS compiler using Java. While the generated
assembly was in text form, it should be simple to adapt it to generate it in binary format, and load that directly
into my emulator.
- As I mentioned above, it would be interesting to wrap the emulator in a virtual world. This was my initial vision
for the project, but life got in the way!
