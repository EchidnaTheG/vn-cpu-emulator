#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/coreutils.h++"
#include <map>

std::map<std::string, uint16_t> opcodes = {
    {"HALT",     0},
    {"LOAD",     1},
    {"STORE",    2},
    {"ADD",      3},
    {"SUB",      4},
    {"INPUT",    5},
    {"OUTPUT",   6},
    {"JUMP",     7},
    {"SKIPZERO", 8},
    {"SKIPPOS",  9},
    {"SKIPERR",  10},
    {"LOADI",    11},
    {"STOREI",   12},
    {"SHIFT",    13},
    {"AND",      14},
    {"XOR",      15},
    {"DATA",     16}
};
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./emulator <filename>\n";
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream file(filename);
    std::string line;
    std::map<std::string, uint16_t> SymbolTable;
    std::vector<std::string> BinaryOutput;
    int count = 1;

    // first pass
    while (std::getline(file, line)){ 
        if (line.empty()) { count ++ ; continue;}
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> tokens;
        while (ss >> segment){
            tokens.push_back(segment);

        }
        if (tokens.empty()){
            count++;
            continue;
        }
        std::string token = tokens[0];
        if (!token.empty() && token.back() == ':'){
            token.pop_back();
            SymbolTable[token] = count;
            count ++;
            continue;
        }
        count++;

    }

    //reset input
    file.clear();
    file.seekg(0, std::ios::beg);

    //second pass
    while (std::getline(file, line)){ 
        if (line.empty()) {continue;}
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> tokens;
        while (ss >> segment){
            tokens.push_back(segment);

        }
        if (tokens.empty()) {continue;}
        std::string token = tokens[0];
        int a = 0 ; int b = 1 ;
        if (!token.empty() && token.back() == ':'){ 
            a++;
            b++;
        }
            std::string opcode = tokens[a];
            std::string address = tokens[b];
            int num_opcode = opcodes[opcode];
            int num_address;
            if (SymbolTable.count(address)){
                num_address = SymbolTable[address];}
            else {
                num_address = std::stoi(address);
            }
            
            if (num_opcode ==  16) {
                std::bitset<16> dataB(num_address);
                std::string finalBinary = dataB.to_string();
                BinaryOutput.push_back(finalBinary);
                continue;
            }
            std::bitset<4> opbits(num_opcode);
            std::cout << "DEBUG OPCODE: " << opcode <<"\n";
            std::bitset<12> adrbits(num_address);
            std::cout << "DEBUG address: " << num_address <<"\n";
            std::string finalBinary = opbits.to_string() + adrbits.to_string();
            BinaryOutput.push_back(finalBinary);
        }
        
        std::ofstream outFile("out.txt");
        for (const auto& binLine : BinaryOutput) {
            outFile << binLine << "\n";
        }
        
        outFile.close();
        std::cout << "Successfully assembled " << BinaryOutput.size() << " instructions.\n";}
        





