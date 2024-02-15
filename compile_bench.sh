#!/bin/bash

# Install dependencies if not already installed
DEPS=("cmake" "libbenchmark-dev" "python2.7" "python2" "git" "libaio-dev")

for dep in "${DEPS[@]}"; do
    if ! dpkg -l | grep -q "^ii  $dep"; then
        echo "Installing $dep..."
        sudo apt-get install -y "$dep"
    else
        echo "$dep is already installed."
    fi
done

# Define the directory
DIR="$(pwd)/compilebench-0.6"

# Check if the compilebench directory exists
if [ -d "$DIR" ]; then
    cd "$DIR"
        
    echo "changing permission"
    sudo chmod +x compilebench
	echo "permission change - complete"
else
    echo "Directory $DIR does not exist."
    exit 1
fi

# Create the required files and set the appropriate permissions
FILES=("compile-result.txt" "compile-output.txt" )

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

# Build Google Benchmark in the compilebench directory
if [ -d "$DIR" ]; then
    cd "$DIR"
    
    # Build Google Benchmark
    echo "Building Benchmark..."
    sudo cmake .
    echo "cmake done"
    sudo make
    echo "make done"
    
    
    # Check if cray_bench is present and executable
    if [ -f "./compile" ] && [ -x "./compile" ]; then
        echo "Running compile..."
	sudo chown $USER:$USER compile
        sudo ./compile --benchmark_counters_tabular=true 
    else
        echo "compile is not found or not executable in $DIR"
        exit 1
    fi
else
    echo "Directory $DIR does not exist."
    exit 1
fi
