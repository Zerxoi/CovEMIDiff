include_directories(
  "include"
  )

add_executable(emi
  main.cpp
  EMIFrontendAction.cpp
  EMIASTConsumer.cpp
  EMIASTVisitor.cpp
  CoverageParser.cpp
  )

target_link_libraries(emi 
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangRewrite
  clangSerialization
  clangTooling
  )