#include <unordered_map>
#include <benchmark/benchmark.h>
#include <string>

#include "TinyMap.h"

using std::string;
using namespace llutils;

static void BM_TinyMapGetEraseAligned(benchmark::State& state) {
    alignas(2) char buffer[64]{};
    static LLTinyMap<10,64>  map(buffer);
    //static std::unordered_map<short, std::string> map;
    for (auto _ : state) {
        short id = rand() % 64;
        string prev = "";
        string val = std::to_string(id);
        void * addr = map.set(id, (const char*)val.c_str(), val.size());
        short id2 = rand() % 64;
        void * addr2 = map.get(id2);
        if (addr2 != nullptr) {
            prev = *((const char*)addr2);
            map.set(id2, (const char*)val.c_str(), val.size());
        }
    }
}
BENCHMARK(BM_TinyMapGetEraseAligned);

BENCHMARK_MAIN();