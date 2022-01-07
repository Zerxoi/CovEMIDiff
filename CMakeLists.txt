cmake_minimum_required(VERSION 3.13.4)
set (CMAKE_CXX_STANDARD 20)
project(EMI)

# prepare: install llvm and clang first
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(
  # add include path of llvm to include directories
  ${LLVM_INCLUDE_DIRS}
  )

## after append ${LLVM_CMAKE_DIR}, include can find AddLLVM.cmake
# list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})
# include(AddLLVM)

## but even if append ${CLANG_CMAKE_DIR}, there is not AddClang.cmake in this folder
# list(APPEND CMAKE_MODULE_PATH ${CLANG_CMAKE_DIR})
# include(AddClang)

add_subdirectory(emi)