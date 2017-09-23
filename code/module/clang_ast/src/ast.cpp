#include "../include/ast.hpp"

#include <fstream>


#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
//#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

#include <clang/Serialization/ASTReader.h>

#include <clang/Frontend/ASTUnit.h>
#include <clang/Frontend/PCHContainerOperations.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>


#include "visitors/call_visitor.hpp"
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace llvm;

std::unique_ptr<clang::ASTUnit> load_AST(const std::string& AST_filename) {

    // TODO HACK options se nepouzivaji
    IntrusiveRefCntPtr<DiagnosticsEngine> Diags(
        new DiagnosticsEngine(
            IntrusiveRefCntPtr<DiagnosticIDs>( new DiagnosticIDs ),
            new DiagnosticOptions
        )
    );
    Diags->setClient(new TextDiagnosticPrinter(llvm::errs(), new DiagnosticOptions));

    RawPCHContainerReader pch_reader;

    return ASTUnit::LoadFromASTFile(
        AST_filename,
        pch_reader,
        Diags,
        FileSystemOptions()
    );
}

#include <clang/AST/ASTImporter.h>

std::unique_ptr<ASTUnit> merge_ASTs(std::vector<std::unique_ptr<ASTUnit>>& ASTs) {
    if( ASTs.empty() ) { return std::unique_ptr<ASTUnit>{}; }
    if( ASTs.size() > 1 ) {

        for(
            auto it = ++( ASTs.begin() );
            it != ASTs.end();
            ++it
        ) {
            // TODO hack importing to front() AST
            clang::ASTImporter Importer(
                ASTs.front()->getASTContext(),
                ASTs.front()->getFileManager(),
                (*it)->getASTContext(),
                (*it)->getFileManager(),
                /*MinimalImport=*/true
            );

            TranslationUnitDecl* TU = (*it)->getASTContext().getTranslationUnitDecl();
            for( auto* D : TU->decls() ) {
                // Don't re-import __va_list_tag, __builtin_va_list.
                if( const auto* ND = dyn_cast<NamedDecl>(D) ) {
                    if( IdentifierInfo* II = ND->getIdentifier() ) {
                        if( II->isStr("__va_list_tag") || II->isStr("__builtin_va_list") ) {
                            continue;
                        }
                    }
                }

                if( Importer.GetAlreadyImportedOrNull(D) == nullptr ) {
                    if( !D->isInStdNamespace() ) {
                        if( dynamic_cast<const DependentScopeDeclRefExpr*>(D) == nullptr ) {
                            Importer.Import(D);
                        }
                    }
                }
            }
        }
    }

    std::unique_ptr<ASTUnit> result;
    std::swap(result, ASTs.front());
    return result;
}
