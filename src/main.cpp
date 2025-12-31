#include <iostream>
#include <coreutils.h++>


int main () {
    RAM ram_ = RAM();
    CPU cpu_ = CPU(&ram_);
    while (cpu_.Status()){
        cpu_.Step();
    }
    return 0;
}