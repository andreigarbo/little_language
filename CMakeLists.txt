cmake_minimum_required(VERSION 3.20.0)
project(LittleLanguage)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCE_DIR "src")

file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")

add_executable(little_compiler ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti")

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(little_compiler LLVMCore LLVMIRReader LLVMBitReader LLVMBitWriter ${llvm_libs})
