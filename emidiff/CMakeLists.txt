include_directories(
  "include"
  )

set(LIBRARY_SOURCES
  EMIDiff.cpp
  DiffParser.cpp
  DiffReason.cpp
  DiffFrontendActionFactory.cpp
  DiffFrontendAction.cpp
  DiffASTConsumer.cpp
  DiffASTVisitor.cpp
  DiffUtil.cpp
  )

add_executable(emidiff
  main.cpp
  ${LIBRARY_SOURCES}
  )

target_link_libraries(emidiff
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  mysqlcppconn
  )