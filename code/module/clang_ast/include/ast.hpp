#pragma once

#include <clang/Frontend/ASTUnit.h>

#include <memory>
#include <string>
#include <vector>


std::unique_ptr<clang::ASTUnit> load_AST(const std::string& AST_filename);

// TODO priserne rozhrani - mutuje se prvni element vektoru
std::unique_ptr<clang::ASTUnit> merge_ASTs(std::vector<std::unique_ptr<clang::ASTUnit>>& ASTs);
