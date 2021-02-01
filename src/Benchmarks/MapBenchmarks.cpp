#include <unordered_map>
#include <benchmark/benchmark.h>
#include <string>

#include "TinyMap.h"
#include "TinyMapT.h"

using std::string;
using namespace llutils;

static void BM_MapCreation(benchmark::State& state) {
    for (auto _ : state)
        std::unordered_map<short,std::string> map;
}
BENCHMARK(BM_MapCreation);

static void BM_MapCopy(benchmark::State& state) {
    std::unordered_map<short, std::string> map;
    for (auto _ : state)
        std::unordered_map<short, std::string> map2(map);
}
BENCHMARK(BM_MapCopy);

static void BM_MapGetSet_nop(benchmark::State& state) {
    for (auto _ : state) {
        unsigned char id = ((unsigned short)rand()) % 64;
        string prev = "";
        string val = std::to_string(id);
        unsigned char id2 = ((unsigned short)rand()) % 64;
    }
}
BENCHMARK(BM_MapGetSet_nop);

static void BM_MapGetSet(benchmark::State& state) {
    static std::unordered_map<short, std::string> map;
    for (auto _ : state) {
        unsigned char id = ((unsigned short)rand()) % 64;
        string prev = "";
        string val = std::to_string(id);
        map[id] = val;
        unsigned char id2 = ((unsigned short)rand()) % 64;
        auto it = map.find(id2);
        if (it != map.end()) {
            map[id2] = prev;
            prev = it->second;
        }
    }
}
BENCHMARK(BM_MapGetSet);

static void BM_TinyMapGetSet(benchmark::State& state) {
    char buffer[64]{};
    static LLTinyMap<10, 64> map(buffer);
    for (auto _ : state) {
        unsigned short id = ((unsigned short)rand()) % 64;
        string prev = "";
        string val = std::to_string(id);
        void * addr = map.set(id, (const char *)val.c_str(), val.size());
        unsigned short id2 = ((unsigned short)rand()) % 64;
        void* addr2 = map.get(id2);
        if (addr2 != nullptr) {
            prev = *((const char *)addr2);
            map.set(id2, (const char*)val.c_str(), val.size());
        }
    }
}
BENCHMARK(BM_TinyMapGetSet);

static void BM_TinyMapGetSetAligned(benchmark::State& state) {
    alignas(2) char buffer[64]{};
    static LLTinyMap<10, 64>  map(buffer);
    for (auto _ : state) {
        unsigned short id = ((unsigned short)rand()) % 64;
        string prev = "";
        string val = std::to_string(id);
        void* addr = map.set(id, (const char*)val.c_str(), val.size());
        unsigned short id2 = ((unsigned short)rand()) % 64;
        void* addr2 = map.get(id2);
        if (addr2 != nullptr) {
            prev = *((const char*)addr2);
            map.set(id2, (const char*)val.c_str(), val.size());
        }
    }
}
BENCHMARK(BM_TinyMapGetSetAligned);

static void BM_TinyMapTGetSet(benchmark::State& state) {
    Kvp32_t buffer[64]{};
    static TinyMapT<10, 64, typename Kvp32_t> map(buffer);
    for (auto _ : state) {
        unsigned short id = ((unsigned short)rand()) % 64;
        string prev = "";
        string val = std::to_string(id);
        //void* addr = map.set(id, (const char*)val.c_str(), val.size());
        int id2 = (rand()) % 64;
        void* addr2 = map.get((unsigned short)id2);
        if (addr2 != nullptr) {
            prev = *((const char*)addr2);
            map.set(id2, (const char*)val.c_str(), val.size());
        }
    }
}
BENCHMARK(BM_TinyMapTGetSet);

static void BM_TinyMapTGetSetAligned(benchmark::State& state) {
    Kvp_t buffer[64]{};
    TinyMapT<10, 64, Kvp_t>  map(buffer);
    for (auto _ : state) {
        alignas(unsigned short) unsigned short id = ((unsigned short)rand()) % 32;
        string prev = "";
        string val = std::to_string(id);
        char * addr = (char*) map.set(id, (const char*)val.c_str(), val.size());
        alignas(unsigned short) unsigned short id2 = ((unsigned short)rand()) % 32;
        char* addr2 = (char*) map.get(id);
    }
}
BENCHMARK(BM_TinyMapTGetSetAligned);


/// *************

static void BM_MapGetErase_nop(benchmark::State& state) {
    static std::unordered_map<short, std::string> map;
    for (auto _ : state) {
        short id = ((unsigned short)rand()) % 64;
        string val = std::to_string(id);
        //map[id] = val;
        short id2 = ((unsigned short)rand()) % 64;
        //auto it = map.find(id2);
        //if (it != map.end()) {
        //    map.erase(it);
        //}
    }
}
BENCHMARK(BM_MapGetErase_nop);

static void BM_MapGetErase(benchmark::State& state) {
    static std::unordered_map<short, std::string> map;
    for (auto _ : state) {
        short id = ((unsigned short)rand()) % 64;
        string val = std::to_string(id);
        map[id] = val;
        short id2 = ((unsigned short)rand()) % 64;
        auto it = map.find(id2);
        if (it != map.end()) {
            map.erase(it);
        }
    }
}
BENCHMARK(BM_MapGetErase);

static void BM_TinyMapGetErase(benchmark::State& state) {
    char buffer[64]{};
    static LLTinyMap<10, 64>  map(buffer);
    for (auto _ : state) {
        unsigned short id = ((unsigned short)rand()) % 64;
        string prev = "";
        string val = std::to_string(id);
        void* addr = map.set(id, (const char*)val.c_str(), val.size());
        unsigned short id2 = ((unsigned short)rand()) % 64;
        void* addr2 = map.get(id2);
        if (addr2 != nullptr) {
            prev = *((const char*)addr2);
            map.set(id2, (const char*)val.c_str(), val.size());
        }
    }
}
BENCHMARK(BM_TinyMapGetErase);

static void BM_TinyMapGetEraseAligned(benchmark::State& state) {
    alignas(2) char buffer[64]{};
    static LLTinyMap<10,64>  map(buffer);
    for (auto _ : state) {
        alignas(unsigned short) unsigned short id = ((unsigned short)rand()) % 64;
        string prev = "";
        string val = std::to_string(id);
        void * addr = map.set(id, (const char*)val.c_str(), val.size());
        alignas(unsigned short) unsigned short id2 = ((unsigned short)rand()) % 64;
        void * addr2 = map.get(id2);
        if (addr2 != nullptr) {
            prev = *((const char*)addr2);
            map.set(id2, (const char*)val.c_str(), val.size());
        }
    }
}
BENCHMARK(BM_TinyMapGetEraseAligned);

BENCHMARK_MAIN();