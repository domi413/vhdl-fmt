[settings]
os=Linux
arch=x86_64
compiler=clang
compiler.version=20
compiler.cppstd=23
build_type=Debug

[conf]
tools.cmake.cmaketoolchain:generator=Ninja
tools.build:compiler_executables={"c": "clang", "cpp": "clang++"}
