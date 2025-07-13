Braze compiler - for 32 bit x86 architecture. Compiles a subset of C.

To build braze compiler with make run:
make clean
make

or use the shell script sh build.sh

Unit tests can be built with Visual Studio MSBuild

Braze compiler depends on various gcc, g++ and nasm libraries. These can be install with install_tools.sh or manually from the command line:

sudo apt-get install -y build-essential gcc make gcc-multilib g++-multilib nasm

#notes and known errors

feature list:
1. local and global variables of primitive type: char, int, short, long, void. R and L values are supported.
2. Function declarations, function prototypes and function calls, all with parameters
3. String support
4. Most operators are implemented, but not logical operators

Known limitations:
1. No pre processor support, include dosn't work. In order to use Printf, declare the following prototype at the top of the file, and nasm will link it: "int printf(const char *format, ...);" 
2. Format specifiers in strings is not supported, example: "test value %d", 10
3. Return keyword is not suppoerted
4. No controll flow constructs are supported like: if, else if, else, for, while, do while
5. No pointer arithmentic is supported 
5. No structs, typedef or complex types are supported

To see examples of implemented language features goto: /unit_test/test_files/codegeneration/
