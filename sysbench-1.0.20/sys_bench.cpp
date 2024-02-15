#include <benchmark/benchmark.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

// Function to get the number of CPU threads
static int GetNumCPUThreads() {
    FILE* pipe = popen("nproc", "r");
    if (!pipe) {
        return -1; // Error opening pipe
    }

    char buffer[128];
    int numThreads = -1; // Default value in case of an error

    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        numThreads = atoi(buffer);
    }

    pclose(pipe);
    return numThreads;
}

// Function to run Sysbench CPU Benchmark
std::string RunSysbenchCPUBenchmark(int iteration) {
    const int numThreads = GetNumCPUThreads();
    std::cout << "Number of CPU Threads: " << numThreads << std::endl;

    if (numThreads <= 0) {
        return "Error getting the number of CPU threads.";
    }

    const int testDuration = 10;  // Set the desired test duration in seconds
    const std::string resultFileName = "sysbench_result.txt";  // File to store the benchmark result
	
	

    // Use append mode for the result file to store each iteration's result
    std::string command = "sudo ./src/sysbench cpu --threads=" + std::to_string(numThreads) +
                          " --time=" + std::to_string(testDuration) + " run >> " + resultFileName;

    system(command.c_str()); // Execute the command

    return resultFileName; // Return the filename to read later
}

// Benchmark function
static void BM_SysbenchCPUBenchmark(benchmark::State& state) {
const std::string resultFileName = "sysbench_result.txt";  // File to store the benchmark result
	
std::ofstream ofs(resultFileName, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    for (auto _ : state) {
        RunSysbenchCPUBenchmark(state.iterations());
    }

    // Parse and compute the average from the saved result files
    std::ifstream resultFile("sysbench_result.txt");
    std::string line;
    std::regex eventsPerSecondRegex("events per second:\\s+(\\d+\\.\\d+)");
    double totalEventsPerSecond = 0.0;
    int numResults = 0;

    while (std::getline(resultFile, line)) {
        std::smatch matches;
        if (std::regex_search(line, matches, eventsPerSecondRegex) && matches.size() > 1) {
            double eventsPerSecond = std::stod(matches[1].str());
            totalEventsPerSecond += eventsPerSecond;
            numResults++;
        }
    }

    // Calculate the average and set it as a user-defined counter
    if (numResults > 0) {
        double averageEventsPerSecond = totalEventsPerSecond / numResults;
        std::cout << "Calculated Average: " << averageEventsPerSecond << " events per second" << std::endl;
        state.counters["AverageEventsPerSecond"] = averageEventsPerSecond;
    }
}

// Registering the benchmark
BENCHMARK(BM_SysbenchCPUBenchmark)->Iterations(3)->Unit(benchmark::kMillisecond);

// Main function for the benchmark
BENCHMARK_MAIN();
