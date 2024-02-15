#include <benchmark/benchmark.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>

double extractMBPerSecond(const std::string& line) {
    // Regular expression pattern to match the MB/s value
    std::regex pattern("\\b(\\d+\\.\\d+)\\sMB/s\\b");
    std::smatch match;
    if (std::regex_search(line, match, pattern)) {
        return std::stod(match[1].str());
    }
    return 0.0; // Default value if not found
}

double extractCompileAvg(const std::string& line) {
    // Regular expression pattern to match the average value
    std::regex pattern("compile total runs (\\d+) avg (\\d+\\.\\d+) MB/s");
    std::smatch match;
    if (std::regex_search(line, match, pattern)) {
        std::cout << "DEBUG: Total runs: " << match[1].str() << ", Average: " << match[2].str() << " MB/s\n";
        return std::stod(match[2].str());
    }
    return 0.0; // Default value if not found
}

// Function to execute the compilebench command and save the results to a file
void runCompilebench() {
    std::system("sudo python2.7 ./compilebench -D t -i 10 --makej COMPILE >> compile-result.txt 2>&1");
}

// Define your custom benchmark function
static void BM_Compilebench(benchmark::State& state) {
    // Clear the file at the beginning
    std::ofstream ofs("compile-result.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    // Execute the compilebench command and save the results
    for (auto _ : state) {
        runCompilebench();
    }
    
    // Check if the file was created successfully
    std::ifstream resultFile("compile-result.txt");
    if (!resultFile) {
        std::cerr << "Failed to open compile-result.txt" << std::endl;
        state.SkipWithError("Failed to open compile-result.txt");
        return;
    }

    std::string line;
    double totalAvg = 0.0;
    int count = 0;
    // Find the line containing "compile total runs"
    while (std::getline(resultFile, line)) {
        if (line.find("compile total runs") != std::string::npos) {
            double mbPerSecond = extractMBPerSecond(line);
            // Set the counter for Google Benchmark
            state.counters["Compile_avg"] = mbPerSecond;

            // Extract the average value and accumulate it
            double compileAvg = extractCompileAvg(line);
            totalAvg += compileAvg;
            count++;
        }
    }

    // Calculate overall average
    if (count > 0) {
        totalAvg /= count;
        // Store the overall average in compile-output.txt
        std::ofstream output("compile-output.txt");
        if (output) {
            output << totalAvg;
            output.close();
        }
    }

    std::ifstream outputFile("compile-output.txt");
    if (outputFile) {
        double avgFromOutputFile;
        outputFile >> avgFromOutputFile;
        state.counters["Compile_avg"] = avgFromOutputFile;
        std::cout << "DEBUG: Average from output file: " << avgFromOutputFile << " MB/s\n";
        outputFile.close();
    } else {
        std::cerr << "Failed to open compile-output.txt" << std::endl;
        state.SkipWithError("Failed to open compile-output.txt");
    }
}

// Register the benchmark function
BENCHMARK(BM_Compilebench)->Iterations(3)->Unit(benchmark::kMillisecond);

// Only one main function, as provided by the benchmark library
BENCHMARK_MAIN();
