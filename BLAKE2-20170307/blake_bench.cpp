#include <benchmark/benchmark.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <regex>
#include <cmath> // Include for std::round

static void BM_Blake2_Benchmark(benchmark::State& state) {
    // Clear the file at the beginning
    std::ofstream ofs("blake_output.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    for (auto _ : state) {
        // Run the BLAKE2 benchmark and append output to a file
        std::system("sudo bench/blake2s >> blake_output.txt");
    }

    // Parse the output file to extract the "#long per byte" value
    std::ifstream file("blake_output.txt");
    std::string line;
    std::regex longPerByteRegex("#long\\s+\\w+\\s+(\\d+\\.\\d+)");
    std::smatch matches;
    double longPerByteValue = 0;
    int count = 0;

    while (getline(file, line)) {
        if (std::regex_search(line, matches, longPerByteRegex) && matches.size() > 1) {
            longPerByteValue += std::stod(matches[1].str());
            count++;
        }
    }

    if (count > 0) {
        longPerByteValue /= count; // Average value if multiple iterations
        longPerByteValue = std::round(longPerByteValue * 1000) / 1000; // Round to 3 decimal places
    }

    // Set the user-defined counter
    state.counters["AveragesPerByte"] = longPerByteValue;
}

// Register the function as a benchmark
BENCHMARK(BM_Blake2_Benchmark)->Iterations(5)->Unit(benchmark::kMillisecond);

// Main function
BENCHMARK_MAIN();
