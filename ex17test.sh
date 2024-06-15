#!/bin/bash

set -e

DB='dbtest.dat'

make ex17
./ex17 $DB c
./ex17 $DB l 
./ex17 $DB s 1 ali ali@test.com
./ex17 $DB s 2 azlan azlan@test.com
./ex17 $DB g 1
./ex17 $DB g 2
./ex17 $DB l
./ex17 $DB d 1
./ex17 $DB l
