#include <benchmark/benchmark.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

// Function to run the Unigine Heaven Benchmark and return FPS value
double RunUnigineHeavenBenchmark() {
    std::cout << "Running Unigine Heaven Benchmark..." << std::endl;

    // Construct and run the Unigine Heaven command
    std::string command = "LD_LIBRARY_PATH=./Unigine_Heaven-4.0/bin/:$LD_LIBRARY_PATH ./Unigine_Heaven-4.0/bin/heaven_x64 -data_path ../ -sound_app null -engine_config ../data/heaven_4.0.cfg -system_script heaven/unigine.cpp -video_mode -1 -extern_define PHORONIX,RELEASE -video_fullscreen >> unigine-result.txt 2>&1";
    std::cout << "Executing command: " << command << std::endl;
    std::system(command.c_str());

    // Read the "unigine-result.txt" to find the FPS value
    std::ifstream resultFile("unigine-result.txt");
    double fps = 0.0;
    if (resultFile.is_open()) {
        std::string line;
        while (std::getline(resultFile, line)) {
            if (line.find("FPS:") != std::string::npos) {
                std::size_t startPos = line.find_first_of("0123456789");
                if (startPos != std::string::npos) {
                    std::size_t endPos = line.find_first_not_of("0123456789.", startPos);
                    std::string fpsString = line.substr(startPos, endPos - startPos);
                    try {
                        fps = std::stod(fpsString);
                    } catch (const std::exception& e) {
                        std::cerr << "Error parsing FPS: " << e.what() << std::endl;
                    }
                }
                break; // Found FPS, no need to continue reading
            }
        }
        resultFile.close();
    } else {
        std::cerr << "Error opening the unigine-result.txt file." << std::endl;
    }

    return fps;
}

static void BM_UnigineHeavenBenchmark(benchmark::State& state) {

	// Clear the file at the beginning
    std::ofstream ofs("unigine-result.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    double sum_fps = 0.0;

    for (auto _ : state) {
        double fps = RunUnigineHeavenBenchmark();
        sum_fps += fps;
    }

    double avg_fps = state.iterations() > 0 ? (sum_fps / state.iterations()) : 0.0;
    avg_fps = std::round(avg_fps * 1000.0) / 1000.0;
    state.counters["AvgFPS"] = avg_fps;
}

BENCHMARK(BM_UnigineHeavenBenchmark)->Iterations(3)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();


/*static void BM_UnigineHeavenBenchmark(benchmark::State& state) {
    // Clear the file at the beginning
    std::ofstream ofs("unigine-result.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    double sum_fps = 0.0;

    for (auto _ : state) {
        state.PauseTiming(); // Pause the timing while setting up the benchmark
        double fps = RunUnigineHeavenBenchmark();
        sum_fps += fps;
        state.ResumeTiming(); // Resume timing for the next iteration
    }

    double avg_fps = state.iterations() > 0 ? (sum_fps / state.iterations()) : 0.0;

    // Round avg_fps to three decimal points
    avg_fps = std::round(avg_fps * 1000.0) / 1000.0;

    state.counters["AvgFPS"] = avg_fps;
}

BENCHMARK(BM_UnigineHeavenBenchmark)->Iterations(1)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();*/
