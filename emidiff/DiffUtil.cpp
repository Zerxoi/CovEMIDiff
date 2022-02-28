#include "DiffUtil.h"

#include <queue>

const clang::Stmt *util::getSiblingStmt(const clang::Stmt *s, int n, clang::ASTContext *Context) {
  if (n == 0) {
    return s;
  }
  auto parents = Context->getParents(*s);
  if (parents.empty()) {
    return nullptr;
  }
  const clang::Stmt *parent = parents.begin()->get<clang::Stmt>();
  auto current = parent->child_begin();
  auto target = parent->child_begin();
  bool reverse = false;
  if (n < 0) {
    reverse = true;
    n = -n;
  }
  while (current != parent->child_end() && n--) {
    current++;
  }
  if (n > 0) {
    return nullptr;
  }
  if (reverse) {
    auto tmp = current;
    current = target;
    target = tmp;
  }
  while (target != parent->child_end() && current != parent->child_end() && *target != s) {
    current++;
    target++;
  }
  if (*target == s) {
    return *current;
  }
  return nullptr;
}

bool util::bfs(const clang::Stmt *root, std::function<bool(const clang::Stmt *)> find) {
  if (root == nullptr) {
    return false;
  }
  std::queue<const clang::Stmt *> queue;
  queue.push(root);
  while (!queue.empty()) {
    auto stmt = queue.front();
    if (find(stmt)) {
      return true;
    }
    if (stmt != nullptr) {
      for (auto child : stmt->children()) {
        queue.push(child);
      }
    }
    queue.pop();
  }
  return false;
}

std::string util::str(const enum coverageTool ct) {
  if (ct == gcov)
    return "gcov";
  else if (ct == llvmcov)
    return "llvm-cov";
  else
    return "";
}