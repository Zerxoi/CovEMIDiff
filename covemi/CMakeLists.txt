include_directories(
  "include"
  )

add_executable(covemi
  main.cpp
  EMIUtil.cpp
  EMIFrontendAction.cpp
  EMIASTConsumer.cpp
  EMIASTVisitor.cpp
  CoverageParser.cpp
  )

target_link_libraries(covemi
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangRewrite
  clangSerialization
  clangTooling
  )