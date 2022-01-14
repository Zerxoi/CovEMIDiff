include_directories(
  "include"
  )

add_executable(diff
  main.cpp
  EMIDiff.cpp
  DiffParser.cpp
  DiffReason.cpp
  DiffFrontendActionFactory.cpp
  DiffFrontendAction.cpp
  DiffASTConsumer.cpp
  DiffASTVisitor.cpp
  Util.cpp
  )

target_link_libraries(diff
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )