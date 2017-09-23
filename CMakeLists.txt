cmake_minimum_required(VERSION 3.0)

project(metaphor CXX)

set(llvm_rootdir    "/home/koja/src/c++/metaphor/project/tools/llvm/")
set(ogre_rootdir    "/home/koja/src/c++/metaphor/project/dependencies/ogre/project/")
set(ois_rootdir     "/home/koja/src/c++/metaphor/project/dependencies/ogre/dependencies/ois/project/")

set(CMAKE_CXX_COMPILER      "${llvm_rootdir}/build/bin/clang++")
set(CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} \
	-std=c++1z -stdlib=libstdc++ \
	-fPIC -fvisibility-inlines-hidden \
	-Wall -W -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wcovered-switch-default -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Werror=date-time -ffunction-sections -fdata-sections \
	-O0 -DNDEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS \
	-I${llvm_rootdir}/include/ -I${llvm_rootdir}/build/include/ \
	-I${llvm_rootdir}/tools/clang/include/ -I${llvm_rootdir}/build/tools/clang/include \
	-DBOOST_LOG_DYN_LINK"
)
set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} \
    -L${ogre_rootdir}/build/lib/    -Wl,-rpath=${ogre_rootdir}/build/lib/   \
    -L${ois_rootdir}/src/.libs/     -Wl,-rpath=${ois_rootdir}/src/.libs/   \
    -L${llvm_rootdir}/build/lib     -Wl,-rpath=${llvm_rootdir}/build/lib"
)

find_package( Boost REQUIRED )

include_directories(
    "${PROJECT_SOURCE_DIR}/code"
    "${Boost_INCLUDE_DIR}"
    "${ogre_rootdir}/build/include/"
    "${ogre_rootdir}/OgreMain/include/"
    "${ogre_rootdir}/Samples/Common/include/"
    "${ogre_rootdir}/Components/Overlay/include/"
    "${ois_rootdir}/includes/"
)

add_library(util_graph STATIC
    code/util/graph/src/bi_graph.cpp
    code/util/graph/src/get_connected_components.cpp
)

add_library(call_graph STATIC
    code/module/call_graph/src/call_graph_builder.cpp
		code/module/call_graph/src/call_graph.cpp
)

target_link_libraries(call_graph
    util_graph
    clang_ast
)

add_library(clang_ast STATIC
    code/module/clang_ast/src/ast.cpp
    code/module/clang_ast/src/get_call_graph_data.cpp
    code/module/clang_ast/src/get_function_info.cpp
    code/module/clang_ast/src/visitors/call_visitor.cpp
)

target_link_libraries(clang_ast
	-Wl,-start-group
		clangFrontend
		clangSerialization
		clangDriver
		clangParse
		clangSema
		clangAnalysis
		clangAST
		clangASTMatchers
		clangBasic
		clangEdit
		clangLex
		clangTooling
		clangToolingCore
		clangFrontendTool
		clang
	-Wl,-end-group
	LLVMLTO
	LLVMPasses
	LLVMObjCARCOpts
	LLVMSymbolize
	LLVMDebugInfoPDB
	LLVMDebugInfoDWARF
	LLVMMIRParser
	LLVMCoverage
	LLVMTableGen
	LLVMOrcJIT
	#LLVMXCoreDisassembler
	#LLVMXCoreCodeGen
	#LLVMXCoreDesc
	#LLVMXCoreInfo
	#LLVMXCoreAsmPrinter
	#LLVMSystemZDisassembler
	#LLVMSystemZCodeGen
	#LLVMSystemZAsmParser
	#LLVMSystemZDesc
	#LLVMSystemZInfo
	#LLVMSystemZAsmPrinter
	#LLVMSparcDisassembler
	#LLVMSparcCodeGen
	#LLVMSparcAsmParser
	#LLVMSparcDesc
	#LLVMSparcInfo
	#LLVMSparcAsmPrinter
    #LLVMPowerPCDisassembler
	#LLVMPowerPCCodeGen
	#LLVMPowerPCAsmParser
	#LLVMPowerPCDesc
	#LLVMPowerPCInfo
	#LLVMPowerPCAsmPrinter
	#LLVMNVPTXCodeGen
	#LLVMNVPTXDesc
	#LLVMNVPTXInfo
	#LLVMNVPTXAsmPrinter
	#LLVMMSP430CodeGen
	#LLVMMSP430Desc
	#LLVMMSP430Info
	#LLVMMSP430AsmPrinter
	#LLVMMipsDisassembler
	#LLVMMipsCodeGen
	#LLVMMipsAsmParser
	#LLVMMipsDesc
	#LLVMMipsInfo
	#LLVMMipsAsmPrinter
	#LLVMLanaiDisassembler
	#LLVMLanaiCodeGen
	#LLVMLanaiAsmParser
	#LLVMLanaiDesc
	#LLVMLanaiInstPrinter
	#LLVMLanaiInfo
	#LLVMHexagonDisassembler
	#LLVMHexagonCodeGen
	#LLVMHexagonAsmParser
	#LLVMHexagonDesc
	#LLVMHexagonInfo
	#LLVMBPFCodeGen
	#LLVMBPFDesc
	#LLVMBPFInfo
	#LLVMBPFAsmPrinter
	#LLVMARMDisassembler
	#LLVMARMCodeGen
	#LLVMARMAsmParser
	#LLVMARMDesc
	#LLVMARMInfo
	#LLVMARMAsmPrinter
	#LLVMAMDGPUDisassembler
	#LLVMAMDGPUCodeGen
	#LLVMAMDGPUAsmParser
	#LLVMAMDGPUDesc
	#LLVMAMDGPUInfo
	#LLVMAMDGPUAsmPrinter
	#LLVMAMDGPUUtils
	#LLVMAArch64Disassembler
	#LLVMAArch64CodeGen
	#LLVMGlobalISel
	#LLVMAArch64AsmParser
	#LLVMAArch64Desc
	#LLVMAArch64Info
	#LLVMAArch64AsmPrinter
	#LLVMAArch64Utils
	LLVMObjectYAML
	LLVMLibDriver
	LLVMOption
	#gtest_main
	#gtest
	LLVMX86Disassembler
	LLVMX86AsmParser
	LLVMX86CodeGen
	LLVMSelectionDAG
	LLVMAsmPrinter
	LLVMDebugInfoCodeView
	#LLVMDebugInfoMSF
	LLVMX86Desc
	LLVMMCDisassembler
	LLVMX86Info
	LLVMX86AsmPrinter
	LLVMX86Utils
	LLVMMCJIT
	LLVMLineEditor
	LLVMInterpreter
	LLVMExecutionEngine
	LLVMRuntimeDyld
	#LLVMCodeGen
	LLVMTarget
	LLVMBitWriter
	#LLVMCoroutines
	LLVMipo
	LLVMInstrumentation
	LLVMVectorize
	LLVMScalarOpts
	LLVMObject
	LLVMMCParser
	LLVMMC
	LLVMLinker
	LLVMIRReader
	LLVMBitReader
	LLVMAsmParser
	LLVMInstCombine
	LLVMTransformUtils
	LLVMAnalysis
	LLVMProfileData
	LLVMCore
	LLVMSupport
	LLVMDemangle
)

add_library(layout STATIC
    code/module/layout/src/create_embedded_box_layout.cpp
    code/module/layout/src/create_spiral_grid_layout.cpp
    code/module/layout/src/harel_koren/harel_koren_3d.cpp
)

add_library(view STATIC
    code/module/view/src/view_factory.cpp
)

add_library(view_ogre3d STATIC
    code/module/view/src/ogre3d/BaseApplication.cpp
    code/module/view/src/ogre3d/ogre_3d_view.cpp
    code/module/view/src/ogre3d/movable_text.cpp
)

target_link_libraries(view_ogre3d
    OgreMain
    OgreOverlay
    OIS
)

add_executable(metaview
    code/app/metaview/metaview.cpp
	code/app/metaview/program_option_parsers.cpp
)

target_link_libraries(metaview
    layout
    clang_ast
    call_graph
    view
    view_ogre3d
    boost_log
		boost_program_options
    boost_system
		rt dl tinfo pthread z m
)

add_library(plugin_dump_ast SHARED
    code/app/plugin_dump_ast.cpp
)

file(COPY ogre_conf/plugins.cfg     DESTINATION .)
file(COPY ogre_conf/resources.cfg   DESTINATION .)
file(COPY ogre_media                DESTINATION .)
file(COPY test_data                 DESTINATION .)

configure_file(clang-wrapper.in  		clang-wrapper)
file(COPY parse_ldmap.py						DESTINATION .)
file(COPY prepare_file_lists.py			DESTINATION .)
