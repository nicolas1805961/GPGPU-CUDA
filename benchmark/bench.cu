#include <benchmark/benchmark.h>

#include "Graph.hpp"
#include "Graph_gpu.hpp"
#include "Image.hpp"
#include "graphcut_cpu.hpp"
#include "graphcut_gpu.hpp"

// Instanciate variables
Image image("../inputs/12003.jpg");
Image imageHelper("../inputs/12003_modified.jpg");

Graph graph(image, imageHelper);
GraphGPU graphgpu(image, imageHelper);

void BM_cpu(benchmark::State& st)
{
    for (auto _ : st) {
        graphcut_cpu(graph);
    }

    st.counters["frame_rate"] = benchmark::Counter(
                                        st.iterations(),
                                        benchmark::Counter::kIsRate
                                );
}

void BM_gpu(benchmark::State& st)
{
    for (auto _ : st) {
        // FIXME : call GPU function here
        graphcut_gpu(graphgpu);
    }

    st.counters["frame_rate"] = benchmark::Counter(
                                        st.iterations(),
                                        benchmark::Counter::kIsRate
                                );
}


BENCHMARK(BM_cpu)
->Unit(benchmark::kMillisecond)
->UseRealTime();

BENCHMARK(BM_gpu)
->Unit(benchmark::kMillisecond)
->UseRealTime();

BENCHMARK_MAIN();
