#pragma once

#include <string>

namespace clang {
    class ASTContext;
    class FunctionDecl;
}

std::string get_function_name(const clang::FunctionDecl& decl);
std::string get_mangled_function_name(const clang::FunctionDecl& decl);
