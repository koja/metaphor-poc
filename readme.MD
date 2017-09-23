# metaphor

Proof of concept C++ source code 3D visualization tool.

## vision

I imagined immersing myself in visualized software in virtual reality, mainly just for the hell of it but also thinking such possibility could be useful.

I wanted to explore other possible representations of software beside linear source code for software reviewing, development, debugging, monitoring, etc. Specifically in C++ are many concepts that might lend itself nicely to visual representation (e. g. namespaces, inheritance, scopes, variable life-time, call-graph, control-flow graph, etc).

## implementation idea

The idea was to use:

1. Clang C++ API to get access to compilation unit AST (just call graphs at the moment)
  - https://clang.llvm.org/docs/IntroductionToTheClangAST.html
2. GNU ld linker to get access to how symbols are linked (unfinished ATM)
  - https://linux.die.net/man/1/ld
3. Fast 3D graph layouting algorithm, Harel-Koren method (working prototype with issues)
  - http://www.wisdom.weizmann.ac.il/~harel/papers/ms_jgaa.pdf
4. Ogre3D game engine to visualize the graph (very basic version implemented)
  - http://www.ogre3d.org/
5. ... later to replace the 3D engine with some VR engine (on wishlist)
  - https://www.khronos.org/openxr (or some higher abstraction level engine)

Parts 1. and 2. uses BASH script wrapper over clang driver with goal of being build system agnostic. Part 1. is implemented as clang plugin dumping full AST to be later found by python helper script. Main binary is currently implementing AST visiting, graph manipulation, layouting (3.) and visualization (4.) in the best tradition of monolithic design. 

## status quo

PoC quality overall, basically just drawing crude graph very loosely correlated to call graph of single C++ source file.

Numerous conceptual, algorithmic and implementation issues. Beware!

Journey to full linked-together-binary file visualization (and/or source code manipulation) using a VR headset is still a long one yet direction for the next few steps is clear.

![Screenshot 1](/doc/examples/screen1.png)
![Screenshot 2](/doc/examples/screen2.png)

## how to build

0. Use Debian based Linux distribution.
1. Install all packages from doc/dependencies.MD
2. Build Llvm 4.0 https://github.com/llvm-mirror/llvm
3. Build Clang 4.0 https://github.com/llvm-mirror/clang
4. Build OIS master branch (commit bb75ccc1aabc1c547195579963601ff6080ca2f2) https://github.com/wgois/OIS/branches
5. Build Ogre3D v1.10.5 https://github.com/OGRECave/ogre/releases
6. Edit llvm_rootdir, ogre_rootdir and ois_rootdir variables in CMakeList.txt 
7. Edit PluginFolder variable in ogre_conf/plugins.cfg
8. Create directory build and run: cd build && cmake .. && make -j

# how to run

Check doc/user_manual.MD
