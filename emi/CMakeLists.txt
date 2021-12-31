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

add_executable(emi
  main.cpp
  EMIFrontendAction.cpp
  EMIASTConsumer.cpp
  EMIASTVisitor.cpp
  CoverageParser.cpp
  )

target_link_libraries(emi PRIVATE ${CLANG_LINK_LIBRARIES})