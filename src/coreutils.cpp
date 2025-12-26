#include <cstdint>
#include <vector>

const int16_t ADD= 3;
const int16_t AND= 5;
const int16_t XOR= 7;
const int16_t SHIFT = 9;
const int16_t SUB = 10;

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
        bool run_flag = false;
        bool aeb = false;
        int16_t ACC = 0;
        u_int16_t PC = 0;
        RAM* pmemory;
    public:
        CPU(RAM* rawMemoryPointer){
            pmemory = rawMemoryPointer;
        }

       void Step(){
            pmemory->read(PC)
        }


    public:

}