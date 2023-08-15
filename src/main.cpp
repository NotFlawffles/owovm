#include "../include/vm.h"
#include <iostream>
#include <fstream>


std::vector<u16> loadFile(std::string fileName) {
    std::ifstream file(fileName, std::ios::in | std::ios::binary);
    u16 size;
    std::vector<u16> program;
    file.read((char*) &size, sizeof(size));
    program.reserve(size);
    program.resize(size);
    file.seekg(sizeof(u16));
    file.read((char*) &program[0], size * sizeof(u16));
    file.close();
    
    return program;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "expected a file name argument." << std::endl;
        return 1;
    }

    std::vector<u16> program = loadFile(argv[1]);
    VM vm;
    vm.loadProgram(program);
    vm.run();
}
