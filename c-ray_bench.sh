#!/bin/bash


# Define the directory
DIR="$(pwd)/c-ray-1.1"

# Check if the c-ray directory exists
if [ -d "$DIR" ]; then
    cd "$DIR"
        
    echo "Building c-ray..."
    sudo cc -o c-ray-mt c-ray-mt.c -lm -lpthread -O3 $CFLAGS
	echo "compilation - complete"
else
    echo "Directory $DIR does not exist."
    exit 1
fi

# Create the required files and set the appropriate permissions
FILES=("c-ray-result.txt" )

for file in "${FILES[@]}"; do
    FILE_PATH="$DIR/$file"
    echo "Checking file $FILE_PATH..."  # Added for debugging
    if [ ! -f "$FILE_PATH" ]; then
        echo "Creating $file..."
        sudo touch "$FILE_PATH"
        sudo chown $USER:$USER "$FILE_PATH"
    else
    	sudo chown $USER:$USER "$FILE_PATH"
        echo "$file already exists."
        
    fi
done

# Build Google Benchmark in the c-ray-1.1 directory
if [ -d "$DIR" ]; then
    cd "$DIR"
    
    # Build Google Benchmark
    echo "Building Benchmark..."
    sudo cmake .
    echo "cmake done"
    sudo make
    echo "make done"
    
    
    # Check if cray_bench is present and executable
    if [ -f "./cray_bench" ] && [ -x "./cray_bench" ]; then
        echo "Running cray_bench..."
	sudo chown $USER:$USER cray_bench
        sudo ./cray_bench --benchmark_counters_tabular=true 
    else
        echo "cray_bench is not found or not executable in $DIR"
        exit 1
    fi
else
    echo "Directory $DIR does not exist."
    exit 1
fi
