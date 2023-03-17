#!/bin/bash

# Check that the correct number of arguments were provided
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 dataset_name num_features poly_degree"
    exit 1
fi

# Extract the arguments into variables
dataset_name=$1
num_features=$2
poly_degree=$3

# Write the output to def.h
echo "#define SAMPLE_SIZE 5000" > src/def.h
echo "#define DATASET $dataset_name" >> src/def.h
echo "#define DATASET_FEATURES $num_features" >> src/def.h
echo "#define POLY_DEGREE $poly_degree" >> src/def.h

#compile and run
gcc -o "eval.out" "src/main.c" -lm -g

./eval.out