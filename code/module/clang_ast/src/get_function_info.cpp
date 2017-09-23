#include "../include/get_function_info.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Mangle.h>

#include <memory>

namespace {
    using namespace clang;
}

std::string get_function_name(const FunctionDecl& decl) {

    return decl.getNameInfo().getName().getAsString();
}


std::string get_mangled_function_name(const FunctionDecl& decl) {

    // TODO
    // http://stackoverflow.com/questions/40740604/how-do-i-get-the-mangled-name-of-a-nameddecl-in-clang
    const std::unique_ptr<MangleContext> mctx{ decl.getASTContext().createMangleContext() };

    if (!mctx->shouldMangleDeclName(&decl)) {
        return decl.getNameInfo().getName().getAsString();
    }

    std::string mangledName;
    llvm::raw_string_ostream ostream(mangledName);

    mctx->mangleName(&decl, ostream);

    ostream.flush();

    return mangledName;
}
