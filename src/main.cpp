#include <iostream>
#include "../include/coreutils.h++"


int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    RAM ram_ = RAM();
    CPU cpu_ = CPU(&ram_);
    std::string line;
    int16_t address = 0;
   while (std::cin >> line) {
        ram_.write(address,std::stoi(line, nullptr, 2));
        address ++;

    }
    std::cin.clear();
    freopen("/dev/tty", "r", stdin);
    while (cpu_.Status()){
        cpu_.Step();
    }
    std::cout << "\nEmulator halted successfully.\n";
    return 0;
}