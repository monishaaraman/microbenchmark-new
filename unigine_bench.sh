#!/bin/bash

# Define the directory
DIR="$(pwd)/unigine-heaven-4.0"

# Check if the unigine-heaven-4.0 directory exists
if [ -d "$DIR" ]; then
    # Enter the unigine-heaven-4.0 directory
    cd "$DIR" || { echo "Failed to navigate to $DIR"; exit 1; }


    # Concatenate the split files into a single tar.gz file
    cat Unigine_Heaven_split* > mergedfile.tar.gz
    echo "merging tar file"

    # Extract the contents of the merged tar.gz file
    sudo tar -xvf mergedfile.tar.gz
    echo "extracted merged file"

    echo "Building unigine-heaven-4.0..."
    chmod +x Unigine_Heaven-4.0.run
    echo "Changing permission"
    ./Unigine_Heaven-4.0.run --nox11
    echo "Compilation - complete"

    # Navigate into the Unigine_Heaven-4.0 directory
    if [ -d "Unigine_Heaven-4.0" ]; then
        cd Unigine_Heaven-4.0 || { echo "Failed to navigate to Unigine_Heaven-4.0 directory"; exit 1; }
        echo "Setting LD_LIBRARY_PATH..."
        export LD_LIBRARY_PATH=$(pwd)/bin/:$LD_LIBRARY_PATH
    else
        echo "Unigine_Heaven-4.0 directory does not exist."
        exit 1
    fi
else
    echo "Directory $DIR does not exist."
    exit 1
fi



# Create the required files and set the appropriate permissions
FILES=("unigine-result.txt")

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

# Build Google Benchmark in the unigine-heaven-4.0 directory
if [ -d "$DIR" ]; then
    cd "$DIR"
    
    # Build Google Benchmark
    echo "Building Benchmark..."
    sudo cmake .
    echo "cmake done"
    sudo make
    echo "make done"
    
    
    # Check if unigine is present and executable
    if [ -f "./unigine" ] && [ -x "./unigine" ]; then
        echo "Running unigine..."
	sudo chown $USER:$USER unigine
        sudo ./unigine --benchmark_counters_tabular=true 
    else
        echo "unigine is not found or not executable in $DIR"
        exit 1
    fi
else
    echo "Directory $DIR does not exist."
    exit 1
fi
