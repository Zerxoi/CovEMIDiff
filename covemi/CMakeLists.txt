include_directories(
  "include"
  )

set(LIBRARY_SOURCES
  EMIUtil.cpp
  EMIFrontendAction.cpp
  EMIASTConsumer.cpp
  EMIASTVisitor.cpp
  CoverageParser.cpp
  )

add_executable(covemi
  main.cpp
  ${LIBRARY_SOURCES}
  )

target_link_libraries(covemi
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangRewrite
  clangSerialization
  clangTooling
  mysqlcppconn
  )