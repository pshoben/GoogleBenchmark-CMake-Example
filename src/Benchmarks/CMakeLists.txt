# CMakeList.txt : CMake project for StringBenchmarks, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)

include(../cmake/GoogleBenchmark.cmake)

add_executable (StringBenchmarks "StringBenchmarks.cpp" "StringBenchmarks.h")
target_link_libraries(StringBenchmarks benchmark::benchmark)

add_executable (MapBenchmarks "MapBenchmarks.cpp")
target_link_libraries(MapBenchmarks benchmark::benchmark)

add_executable (MapBenchmarksEraseAligned "MapBenchmarksEraseAligned.cpp")
target_link_libraries(MapBenchmarksEraseAligned benchmark::benchmark)

add_executable (MapBenchmarksEraseNonAligned "MapBenchmarksEraseNonAligned.cpp")
target_link_libraries(MapBenchmarksEraseNonAligned benchmark::benchmark)

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
    target_link_libraries(StringBenchmarks Shlwapi)
    target_link_libraries(MapBenchmarks Shlwapi)
endif()

