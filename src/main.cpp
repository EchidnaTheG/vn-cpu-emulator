#include <iostream>
#include <fstream>
#include "../include/coreutils.h++"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./emulator <filename>\n";
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: could not open file: " << argv[1] << "\n";
        return 1;
    }
    RAM ram_ = RAM();
    CPU cpu_ = CPU(&ram_);
    std::string line;
    int16_t address = 0;
   while (std::getline(file, line)) {
        ram_.write(address,std::stoi(line, nullptr, 2));
        address ++;

    }
    while (cpu_.Status()){
        cpu_.Step();
    }
    std::cout << "\nEmulator halted successfully.\n";
    return 0;
}