#!/bin/bash
gcc -c -Wall -fPIC -D_GNU_SOURCE add.c
gcc add.o -shared -o lib_dynadd.so
gcc -c program.c -o program.o
gcc program.o -L. -l_dynadd -o binary.out
