
cmake_minimum_required(VERSION 3.4.1)

add_library(native-lib SHARED native-lib.cpp AudioEngine.cpp SampleFileManager.cpp)

find_library(
        log-lib log android)

# Build the Oboe library
set(OBOE_DIR ./oboe)
add_subdirectory(${OBOE_DIR} ./oboe)

# Make the Oboe public headers available to our app
include_directories(${OBOE_DIR}/include)

target_link_libraries(native-lib ${log-lib} oboe android)