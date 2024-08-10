#include <iostream>
#include "compilerProcess.h"

int main()
{
    std::cout << "Braze Compiler\n";
    compileProcess process;
    process.intialize();
    process.startCompiler();
}
