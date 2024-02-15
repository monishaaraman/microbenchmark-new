#!/bin/bash

# Start GpuTest command in the background
./GpuTest /test=fur /width=800 /height=600 /fullscreen /benchmark /benchmark_duration_ms=50000 /no_scorebox /disable_opengl_support_check
# Capture the PID of the last background process
#PID=$!

#echo GpuTest started with PID: $PID

# Wait for 60 seconds
#sleep 80

# Get the process name associated with the PID
#PROCESS_NAME=$(ps -p $PID -o comm=)

# Print the process name
#echo Process name: $PROCESS_NAME

# Kill the GpuTest process
#kill $PID
#echo GpuTest process ($PID) has been terminated






