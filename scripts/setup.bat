@ECHO OFF
git submodule update --remote --merge
MKDIR build
PUSHD build
cmake ..
POPD
@ECHO ON