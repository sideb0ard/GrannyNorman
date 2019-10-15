
cmake_minimum_required(VERSION 3.4.1)


add_library(native-lib SHARED native-lib.cpp AudioEngine.cpp WavData.cpp LinkManager.cpp)

find_library(
        log-lib log android)

# Build the Oboe library
set(OBOE_DIR ./oboe)
add_subdirectory(${OBOE_DIR} ./oboe)
# Make the Oboe public headers available to our app
include_directories(${OBOE_DIR}/include)


# Ableton Link
set(LINK_DIR /Users/sideboard/Code/link)
include(${LINK_DIR}/AbletonLinkConfig.cmake)
include_directories(${LINK_DIR}/include)
include_directories(${LINK_DIR}/modules/asio-standalone/asio/include)

target_compile_definitions(native-lib PRIVATE
        ANDROID=1 LINK_PLATFORM_LINUX=1 PD=1 ABL_LINK_OFFSET_MS=23)

target_link_libraries(native-lib
        ${log-lib}
        oboe
        android
        Ableton::Link
        )