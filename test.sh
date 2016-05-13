#!/bin/bash


programName=$1
testFilesDir=$2


echo "TESTING PROGRAM: $programName WITH TEST FILES IN DIR: $testFilesDir/"

failures=0

    for input in ${testFilesDir}/*.in; do
        echo ""
        echo "TESTING ${input%.*}..."
        ./${programName%.*} < ${input} > temp_stdout.out 2>temp_stderr.out
        diff -q "${input%.*}.out" temp_stdout.out 1>/dev/null
        if [[ $? == "0" ]]
        then
            continue #echo "STANDARD   OUT: SUCCESS"
        else
            echo "STANDARD   OUT: FAILURE"
            ((failures++))
        fi
        diff -q "${input%.*}.err" temp_stderr.out 1>/dev/null
        if [[ $? == "0" ]]
        then
            continue #echo "DIAGNOSTIC OUT: SUCCESS"
        else
            echo "DIAGNOSTIC OUT: FAILURE"
            ((failures++))
        fi
    done
    rm temp_stdout.out
    rm temp_stderr.out

echo "TESTS FAILED: ${failures}"