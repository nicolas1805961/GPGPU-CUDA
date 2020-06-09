#include <benchmark/benchmark.h>


void BM_cpu(benchmark::State& st)
{
    for (auto _ : st) {
        // FIXME : call CPU function here
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
