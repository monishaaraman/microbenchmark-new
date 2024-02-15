#include <benchmark/benchmark.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// Function to execute a benchmark script and append the result to a specified file
void RunAndAppendBenchmark(const std::string& scriptName, const std::string& resultFileName, const std::regex& scoreRegex) {
    if (std::system(scriptName.c_str()) != 0) {
        std::cerr << "Error executing benchmark script: " << scriptName << std::endl;
        return;
    }

    std::ifstream logFile("_geeks3d_gputest_log.txt");
    std::string line;

    if (logFile.is_open()) {
        while (std::getline(logFile, line)) {
            std::smatch matches;
            if (std::regex_search(line, matches, scoreRegex) && matches.size() > 1) {
                std::ofstream resultFile(resultFileName, std::ios_base::app); // Append mode
                if (resultFile.is_open()) {
                    resultFile << matches[1].str() << std::endl;
                    resultFile.close();
                } else {
                    std::cerr << "Error opening " << resultFileName << " for appending." << std::endl;
                }
                return;
            }
        }
        logFile.close();
    } else {
        std::cerr << "Error opening _geeks3d_gputest_log.txt." << std::endl;
    }
}

// Function to calculate the average score from a results file
double CalculateAverageScore(const std::string& resultFileName) {
    std::ifstream resultFile(resultFileName);
    std::string line;
    std::vector<int> scores;

    while (std::getline(resultFile, line)) {
        try {
            scores.push_back(std::stoi(line));
        } catch (const std::exception& e) {
            std::cerr << "Error reading score from " << resultFileName << ": " << e.what() << std::endl;
        }
    }

    resultFile.close();

    if (scores.empty()) return 0.0;

    double sum = 0.0;
    for (int score : scores) {
        sum += score;
    }
    return sum / scores.size();
}

// Function to clear a results file
void ClearResultsFile(const std::string& resultFileName) {
    std::ofstream ofs(resultFileName, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    if (!ofs) {
        std::cerr << "Failed to clear " << resultFileName << std::endl;
    }
}

// Benchmark for FurMark
static void BM_FurMarkBenchmark(benchmark::State& state) {
    const std::string furMarkScript = "/home/monisha/Downloads/GpuTest_Linux_x64_0.7.0/start_furmark_benchmark_fullscreen_1920x1080.sh";
    const std::string furMarkResultFile = "furmark_res.txt";
    const std::regex furMarkScoreRegex(R"(\[Benchmark_Score\] - module: FurMark - Score: (\d+) points)");

    ClearResultsFile(furMarkResultFile);

    for (auto _ : state) {
        RunAndAppendBenchmark(furMarkScript, furMarkResultFile, furMarkScoreRegex);
    }

    state.PauseTiming();
    double averageScore = CalculateAverageScore(furMarkResultFile);
    state.counters["AverageFurMarkScore"] = averageScore;
    state.ResumeTiming();
}

// Benchmark for TessMark
static void BM_TessMarkBenchmark(benchmark::State& state) {
    const std::string tessMarkScript = "/home/monisha/Downloads/GpuTest_Linux_x64_0.7.0/start_tessmark_benchmark_fullscreen_1920x1080.sh";
    const std::string tessMarkResultFile = "tessmark_res.txt";
    std::regex tessMarkScoreRegex(R"(\[Benchmark_Score\] - module: TessMark X\d+ - Score: (\d+) points)");

    ClearResultsFile(tessMarkResultFile);

    for (auto _ : state) {
        RunAndAppendBenchmark(tessMarkScript, tessMarkResultFile, tessMarkScoreRegex);
    }

    state.PauseTiming();
    double averageScore = CalculateAverageScore(tessMarkResultFile);
    state.counters["AverageTessMarkScore"] = averageScore;
    state.ResumeTiming();
}

// Register benchmarks
BENCHMARK(BM_FurMarkBenchmark)->Iterations(3)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_TessMarkBenchmark)->Iterations(3)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
