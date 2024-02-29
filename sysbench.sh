#!/bin/bash
 

# Define the sysbench directory
DIR="$(pwd)/sysbench-1.0.20"

# Check if the sysbench directory already exists
if [ -d "$DIR" ]; then
    echo "Sysbench directory already exists. Skipping extraction."
else
    echo "Extracting sysbench-1.0.20.zip..."
    # unzip the sysbench directory
    sudo unzip sysbench-1.0.20.zip 
fi

# Define the path to the sysbench binary
SYSBENCH_BIN="$DIR/src/sysbench"

# Check if the sysbench binary exists
if [ -f "$SYSBENCH_BIN" ]; then
    echo "Sysbench binary found."
else
    echo "Sysbench binary not found. Building sysbench..."

    # Change to the sysbench source directory
    cd "$DIR"

    # Run the build commands
    sudo ./autogen.sh
    echo "autogen done"
    
    sudo ./configure --without-mysql
    echo "configure done"
    
    sudo make
    echo "make done"
    
    # Check if the build was successful
    if [ -f "$SYSBENCH_BIN" ]; then
        echo "Sysbench successfully built."
    else
        echo "Failed to build sysbench."
        exit 1
    fi
fi

# Create the required file and set the appropriate permissions
FILE_PATH="$DIR/sysbench_result.txt"
if [ ! -f "$FILE_PATH" ]; then
    echo "Creating sysbench_result.txt..."
    sudo touch "$FILE_PATH"
    sudo chown $USER:$USER "$FILE_PATH"
else
    sudo chown $USER:$USER "$FILE_PATH"
    echo "sysbench_result.txt already exists."
fi

# Build Google Benchmark in the sysbench-1.0.20 directory
if [ -d "$DIR" ]; then
    cd "$DIR"
    
    # Build Google Benchmark
    echo "Building Benchmark..."
    sudo cmake .
    echo "cmake done"
    sudo make
    echo "make done"
    
    
    # Check if sys_bench is present and executable
    if [ -f "./sys_bench" ] && [ -x "./sys_bench" ]; then
        echo "Running sys_bench..."
	sudo chown $USER:$USER sys_bench
        sudo ./sys_bench --benchmark_counters_tabular=true 
    else
        echo "sys_bench is not found or not executable in $DIR"
        exit 1
    fi
else
    echo "Directory $DIR does not exist."
    exit 1
fi
