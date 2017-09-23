#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"

#include <clang/Serialization/ASTWriter.h>
#include <llvm/Bitcode/BitstreamWriter.h>

#include <fstream>

using namespace clang;

namespace {

struct MyASTConsumer : public ASTConsumer {
    CompilerInstance& compiler_instance;
    const std::string input_file_path;
    const std::string output_file_path;

    MyASTConsumer(
        CompilerInstance& _compiler_instance,
        const std::string& _input_file_path,
        const std::string& _output_file_path
    ) :
        compiler_instance(_compiler_instance),
        input_file_path(_input_file_path),
        output_file_path(_output_file_path)
    {  }

    void HandleTranslationUnit(ASTContext& ctx) override {

        SmallString<128> buffer;
        llvm::BitstreamWriter bitstream_writer(buffer);

        ASTWriter ast_writer(
            bitstream_writer, {}, true
        );

        const Decl* const trans_unit_decl = ctx.getTranslationUnitDecl();

        clang::Module* const module =
            trans_unit_decl->getImportedOwningModule() != nullptr
                ?   trans_unit_decl->getImportedOwningModule()
                :   trans_unit_decl->getLocalOwningModule();

        ast_writer.WriteAST(
            compiler_instance.getSema(),
            input_file_path,
            module,
            ""
        );

        bitstream_writer.FlushToWord();

        std::ofstream output(output_file_path);

        for (const auto& ch : buffer) {
            output << ch;
        }
    }
};

class DumpAstAction : public PluginASTAction {
    protected:
        std::unique_ptr<ASTConsumer> CreateASTConsumer(
            CompilerInstance& CI,
            llvm::StringRef input_file
        ) override {
            return llvm::make_unique<MyASTConsumer>(CI, input_file, input_file.str() + ".ast");
        }

        bool ParseArgs(
            const CompilerInstance& CI,
            const std::vector<std::string>& args
        ) override {

            return true;
        }
};

}

static FrontendPluginRegistry::Add<DumpAstAction> X("generate-binary-ast-file", "generate binary AST file");