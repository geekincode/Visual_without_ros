cmake_minimum_required(VERSION 3.20)
project(foxglove_slam)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 首先检查项目源目录下 thirdparty 中的 Foxglove SDK（由脚本手动下载到此处）
set(FOXGLOVE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/foxglove-src")

if(EXISTS "${FOXGLOVE_SRC_DIR}/include/foxglove/server.hpp")
    message(STATUS "Using existing Foxglove SDK from: ${FOXGLOVE_SRC_DIR}")
    set(FOXGLOVE_SDK_INCLUDE_DIR "${FOXGLOVE_SRC_DIR}/include")
    set(FOXGLOVE_SDK_LIBRARY "${FOXGLOVE_SRC_DIR}/lib/libfoxglove.a")
    
    # 包含Foxglove SDK源文件
    file(GLOB FOXGLOVE_SOURCES CONFIGURE_DEPENDS
        "${FOXGLOVE_SRC_DIR}/src/*.cpp"
        "${FOXGLOVE_SRC_DIR}/src/server/*.cpp"
    )
else()
    message(FATAL_ERROR "Foxglove SDK not found at: ${FOXGLOVE_SRC_DIR}\nPlease run the provided helper script to download and prepare the SDK, for example:\n  bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/fetch_and_prepare_foxglove.sh \\\n    https://github.com/foxglove/foxglove-sdk/releases/download/sdk%2Fv0.16.2/foxglove-v0.16.2-cpp-x86_64-unknown-linux-gnu.zip \\\n    12ccf93169a800496d7e0f4428127e92cb00e1862a3e609f141a2aaae0c8946f \\\n    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/foxglove-src\nThen re-run CMake.")
endif()

# Find required packages
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

# Find PCL
find_package(PCL REQUIRED)

# Get Protobuf
find_package(Protobuf REQUIRED)
if(NOT PROTOBUF_FOUND)
    message(FATAL_ERROR "Protobuf is required")
endif()

# Include PCL directories
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

# Define the main executable
add_executable(slam_foxglove
    src/main.cpp
    src/slam_processor.cpp
    src/foxglove_bridge.cpp
)

# Add include directory for Foxglove SDK
target_include_directories(slam_foxglove PRIVATE ${FOXGLOVE_SDK_INCLUDE_DIR})
target_include_directories(slam_foxglove PRIVATE ${FOXGLOVE_SDK_INCLUDE_DIR}/foxglove)

# Add Foxglove SDK source files
target_sources(slam_foxglove PRIVATE ${FOXGLOVE_SOURCES})

# Link against Foxglove SDK
target_link_libraries(slam_foxglove 
    ${FOXGLOVE_SDK_LIBRARY}
    ${Protobuf_LIBRARIES}
    Threads::Threads
    ${Boost_LIBRARIES}
    ${PCL_LIBRARIES}
)

# Set include directories for target
target_include_directories(slam_foxglove PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROTOBUF_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)