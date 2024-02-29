#!/bin/bash


# Define the directory
DIR="$(pwd)/GpuTest_Linux_x64_0.7.0"

# Check if the Gputest directory exists
if [ -d "$DIR" ]; then
    cd "$DIR"
        
    echo "changing permission"
    chmod +x GpuTest
	echo "compilation - complete"
else
    echo "Directory $DIR does not exist."
    exit 1
fi

# Create the required files and set the appropriate permissions
FILES=("furmark_res.txt" "tessmark_res.txt" )

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

# Build Google Benchmark in the GpuTest directory
if [ -d "$DIR" ]; then
    cd "$DIR"
    
    # Build Google Benchmark
    echo "Building Benchmark..."
    sudo cmake .
    echo "cmake done"
    sudo make
    echo "make done"
    
    
    # Check if furmark_bench is present and executable
    if [ -f "./furmark_bench" ] && [ -x "./furmark_bench" ]; then
        echo "Running furmark_bench..."
	sudo chown $USER:$USER furmark_bench
        sudo ./furmark_bench --benchmark_counters_tabular=true 
    else
        echo "furmark_bench is not found or not executable in $DIR"
        exit 1
    fi
else
    echo "Directory $DIR does not exist."
    exit 1
fi
