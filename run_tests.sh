#!/bin/sh
# Compiles & runs tests, stripping away deprecated GCC warnings
make
make tests 2>&1 >/dev/null | grep -v -e '^/var/folders/*' -e '^[[:space:]]*\.section' -e '^[[:space:]]*\^[[:space:]]*~*'
./bin/tests
