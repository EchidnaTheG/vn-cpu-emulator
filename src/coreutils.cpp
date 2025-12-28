#include <cstdint>
#include <vector>
#include <iostream>
#define ADD 3
#define AND 5
#define XOR 7
#define SHIFT  9
#define SUB 4
const std::vector<int16_t> ALUOPS {3,5,7,9,10};

int16_t ALU(int16_t acc_val, int16_t op_code, int16_t memory_val, bool &aeb_flag){
    switch (op_code)
    {
        case ADD:
        int16_t result = (int16_t)acc_val + (int16_t)memory_val;
        if (result > 32767 || result < -32767) {aeb_flag = true;}
        return result; 

        case AND:
            return (int16_t)acc_val & (int16_t)memory_val;

        case SUB:
        int16_t result = (int16_t)acc_val - (int16_t)memory_val;
        if (result > 32767 || result < -32767) {aeb_flag = true;}
        return result; 
        case XOR:
            return (int16_t)acc_val ^ (int16_t)memory_val;
        case SHIFT:
            if (memory_val >= 0)
            {
                return (int16_t)(acc_val << memory_val);
            }
            else
            {
                return (int16_t)(acc_val >> -memory_val);
            }

        default:
            break;
        }
 }

 class RAM{

    private:
    std::vector<u_int16_t> memory;
    public:

    RAM(){
        memory.resize(4096, 0);
    }

    uint16_t read(uint16_t address) {
        if (address >= 4096) return 0; // Bounds checking
        return memory[address];
    }

    void write(uint16_t address, uint16_t value) {
        if (address < 4096) {
            memory[address] = value;
        }
    }
};

class CPU{
    private:
        bool run_flag = true;
        bool aeb = false;
        int16_t ACC = 0;
        u_int16_t PC = 0;
        RAM* pmemory;
    public:
        CPU(RAM* rawMemoryPointer){
            pmemory = rawMemoryPointer;
        }

       void Step(){
            u_int16_t ins = pmemory->read(PC);
            u_int16_t opcode = ins >> 12 ;
            u_int16_t ins_address = ((ins << 4) >> 4) | 4095; // 0000 0000 0000 0000
            switch (opcode)           
            {
            case 0:
                run_flag = false;
                break;
            case 1:
                ACC = pmemory->read(ins_address);
                PC += 1;
                break;
            case 2:
                pmemory->write(ins_address, ACC);
                PC += 1;
                break;
            case 3:
                ACC= ALU(ACC, opcode, pmemory->read(ins_address),aeb);
                PC += 1;
                break;
            case 4:
                ACC= ALU(ACC, opcode, pmemory->read(ins_address),aeb);
                PC += 1;
                break;
            case 5:
                std::cin >> ACC; // needs stricter input checking, has to be numeric, ok for now
                PC += 1;
                break;
            case 6:
                std::cout << ACC;
                PC += 1;
                break;
            case 7:
                PC = pmemory->read(ins_address);
                break;
            case 8:
                if (ACC == 0) { 
                    PC += 2 ;
                    break;
                } 
                PC+= 1;
                break;
            case 9:
                if (ACC > 0) { 
                    PC += 2 ;
                    break;
                } 
                PC+= 1;
                break;
            case 10:
                if (aeb) { 
                    PC += 2 ;
                    break;
                } 
                PC+= 1;
                break;
            case 11:
               u_int16_t target_address = pmemory->read(ins_address); // must verify that there are no shenanigans when it comes to types
                ACC = pmemory->read(target_address);
                PC += 1;
                break;
            case 12:
            
            default:
                break;
            }

        }


    public:

}