#!/bin/bash

# Compile the C code with GCC
gcc main.c Queue.c linked_list.c -o exec 

# Check if compilation was successful
if [ $? -eq 0 ]; then
    # Run the compiled program
    ./exec -i test_case_2.csv 
else
    echo "Compilation failed."
fi

