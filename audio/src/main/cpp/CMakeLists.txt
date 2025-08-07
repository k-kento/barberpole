cmake_minimum_required(VERSION 3.22.1)

project(audio-lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # 必須にする

add_library (audio-lib SHARED audio_engine.cpp jni_bridge.cpp wav_loader.cpp)

find_package (oboe REQUIRED CONFIG)

target_link_libraries(
        audio-lib
        log
        oboe::oboe
        android
)