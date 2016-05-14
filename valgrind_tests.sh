#!/bin/bash


programName=$1
testFilesDir=$2


echo "VALGRIND TEST - PROGRAM: $programName WITH TEST FILES IN DIR: $testFilesDir/"
    rm valgrind.out

    for input in ${testFilesDir}/*.in; do
        echo ""
        echo "TESTING ${input%.*}..."
        valgrind ./${programName%.*} < ${input} >/dev/null 2>>valgrind.out
    done