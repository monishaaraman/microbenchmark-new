#include <benchmark/benchmark.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

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

std::string RunCRayBenchmark() {
    const int numThreads = GetNumCPUThreads();
    if (numThreads <= 0) {
        return "Error getting the number of CPU threads.";
    }

	const int RT_THREADS = numThreads * 16;  // Multiply the number of CPU threads by 16

    const std::string resolution = "3840x2160";  // Set the desired resolution
    const int rayCount = 16;  // Set the desired ray count
    const std::string inputScene = "sphfract";  // Set the input scene
    const std::string outputFileName = "output.ppm";  // Set the output file name

    // Redirect the output of the command to c-ray-result.txt
    std::string command = "./c-ray-mt -t " + std::to_string(numThreads) +
                          " -s " + resolution +
                          " -r " + std::to_string(rayCount) +
                          " -i " + inputScene +
                          " -o " + outputFileName + " > c-ray-result.txt 2>&1";

    std::system(command.c_str());

    // Read the "c-ray-result.txt" to find the rendering time output
    std::ifstream resultFile("c-ray-result.txt");
    std::string line, renderingTimeOutput;
    if (resultFile.is_open()) {
        while (std::getline(resultFile, line)) {
            if (line.find("Rendering took:") != std::string::npos) {
                renderingTimeOutput = line;
                break;
            }
        }
        resultFile.close();
    } else {
        return "Error opening the c-ray-result.txt file.";
    }

    return renderingTimeOutput;
}

static void BM_CRayBenchmark(benchmark::State& state) {
    for (auto _ : state) {
        std::string renderingTimeOutput = RunCRayBenchmark();

        // Extract the rendering time in milliseconds
        std::size_t startPos = renderingTimeOutput.find("(");
        std::size_t endPos = renderingTimeOutput.find(" milliseconds)");
        if (startPos != std::string::npos && endPos != std::string::npos) {
            std::string timeStr = renderingTimeOutput.substr(startPos + 1, endPos - startPos - 1);
            try {
                int renderingTime = std::stoi(timeStr);
                state.counters["AverageRenderingtime"] = renderingTime;
            } catch (const std::exception& e) {
                std::cerr << "Error parsing rendering time: " << e.what() << std::endl;
                state.SkipWithError("Error parsing rendering time");
            }
        } else {
            std::cerr << "Could not parse rendering time from the output." << std::endl;
            state.SkipWithError("Error parsing rendering time from the output");
        }
    }
}

BENCHMARK(BM_CRayBenchmark)->Iterations(1)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
