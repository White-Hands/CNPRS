#!/bin/bash
gcc -o cnpr2 cnpr2.c -I /usr/local/include/pbc -L. -lpbc -lgmp
./cnpr2 < ../a.param

