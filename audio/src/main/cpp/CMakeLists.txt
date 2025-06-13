cmake_minimum_required(VERSION 3.22.1)

project(native-lib)

# Build our own native library
add_library (native-lib SHARED AudioEngine.cpp)

# Find the Oboe package
find_package (oboe REQUIRED CONFIG)

# Specify the libraries which our native library is dependent on, including Oboe
target_link_libraries(native-lib log oboe::oboe)