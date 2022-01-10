include_directories(
  "include"
  )

add_executable(diff
  main.cpp
  EMIDiff.cpp
  DiffFrontendActionFactory.cpp
  DiffFrontendAction.cpp
  DiffASTConsumer.cpp
  DiffASTVisitor.cpp
  )

target_link_libraries(diff
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )