#!/bin/bash

# Get the script's directory as the base directory
BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Define the directories relative to the script's location
DIR="$BASE_DIR/BLAKE2-20170307/bench"
FILEDIR="$BASE_DIR/BLAKE2-20170307"

# Check if the BLAKE2-20170307/bench directory exists
if [ -d "$DIR" ]; then
    cd "$DIR"
    
    # Check if blake2s is already built
    if [ ! -f "blake2s" ]; then
        echo "Building blake2..."
        sudo make
        echo "compilation - complete"
    else
        echo "blake2s is already built in $DIR"
    fi
else
    echo "Directory $DIR does not exist."
    exit 1
fi

# Create the required files and set the appropriate permissions
FILES=("blake_output.txt")

for file in "${FILES[@]}"; do
    FILE_PATH="$FILEDIR/$file"
    echo "Checking file $FILE_PATH..."
    if [ ! -f "$FILE_PATH" ]; then
        echo "Creating $file..."
        sudo touch "$FILE_PATH"
        sudo chown $USER:$USER "$FILE_PATH"
    else
        sudo chown $USER:$USER "$FILE_PATH"
        echo "$file already exists."
    fi
done

# Build Google Benchmark in the BLAKE2-20170307 directory
if [ -d "$FILEDIR" ]; then
    cd "$FILEDIR"
    
    # Build Google Benchmark
    echo "Building Benchmark..."
    sudo cmake .
    echo "cmake done"
    sudo make
    echo "make done"
    
    # Check if blake_bench is present and executable
    if [ -f "./blake_bench" ] && [ -x "./blake_bench" ]; then
        echo "Running blake_bench..."
        sudo chown $USER:$USER blake_bench
        sudo ./blake_bench --benchmark_counters_tabular=true 
    else
        echo "blake_bench is not found or not executable in $FILEDIR"
        exit 1
    fi
else
    echo "Directory $FILEDIR does not exist."
    exit 1
fi
