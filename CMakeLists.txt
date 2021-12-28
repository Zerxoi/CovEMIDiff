cmake_minimum_required(VERSION 3.13.4)
project(EMI)

# prepare: install llvm and clang first
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  # add include path of llvm to include directories
  ${LLVM_INCLUDE_DIRS}
  )

## after append ${LLVM_CMAKE_DIR}, include can find AddLLVM.cmake
# list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})
# include(AddLLVM)

## but even if append ${CLANG_CMAKE_DIR}, there is not AddClang.cmake in this folder
# list(APPEND CMAKE_MODULE_PATH ${CLANG_CMAKE_DIR})
# include(AddClang)

set(CLANG_LINK_LIBRARIES
  libclang
  clangTooling
  clangARCMigrate
  clangAST
  clangASTMatchers
  clangAnalysis
  clangBasic
  clangCodeGen
  clangDriver
  clangEdit
  clangFrontend
  clangFrontendTool
  clangLex
  clangParse
  clangRewrite
  clangRewriteFrontend
  clangSema
  clangSerialization
  clangCodeGen
  )

function(clang_target_link target type)
if (CLANG_LINK_CLANG_DYLIB)
  target_link_libraries(${target} ${type} clang_shared)
else()
  target_link_libraries(${target} ${type} ${ARGN})
endif()
endfunction()

add_executable(${PROJECT_NAME}
  main.cpp
  EMIFrontendAction.cpp
  EMIASTConsumer.cpp
  EMIASTVisitor.cpp
  CoverageParser.cpp
  )

clang_target_link(${PROJECT_NAME} PRIVATE ${CLANG_LINK_LIBRARIES})