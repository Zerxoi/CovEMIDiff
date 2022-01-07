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

include_directories(
  "include"
  )

add_executable(emi
  main.cpp
  EMIASTVisitor.cpp
  EMIASTConsumer.cpp
  CoverageParser.cpp
  EMIFrontendAction.cpp
  )

target_link_libraries(emi PRIVATE ${CLANG_LINK_LIBRARIES})