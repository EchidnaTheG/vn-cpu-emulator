#include <cstdint>
#include <vector>
#include <iostream>
#include <climits>
#include "../include/coreutils.h++"





int16_t ALU(int16_t acc_val, int16_t op_code, int16_t memory_val, bool &aeb_flag){
    switch (op_code)
    {
        case ADD_:{
        int32_t temp = acc_val + memory_val;
        if (temp > INT16_MAX || temp < INT16_MIN) {aeb_flag = true;}
        int16_t result = (int16_t)temp;
        return result;}
        

        case AND_:{
            return (int16_t)acc_val & (int16_t)memory_val;}

        case SUB_:{
        int32_t temp = acc_val - memory_val;
        if (temp > INT16_MAX || temp < INT16_MIN) {aeb_flag = true;}
        int16_t result = (int16_t)temp;
        return result;}

        case XOR_:{
            return (int16_t)acc_val ^ (int16_t)memory_val;}
        case SHIFT_:{
            if (memory_val >= 0)
            {
                return (int16_t)(acc_val << memory_val);
            }
            else
            {
                return (int16_t)(acc_val >> -memory_val);
            }
        }
        default:
            return acc_val;

        }
 }



    RAM::RAM(){
        memory.resize(4096, 0);
    }

    uint16_t RAM::read(uint16_t address) {
        if (address >= 4096) return 0; // Bounds checking
        return memory[address];
    }

    void RAM::write(uint16_t address, uint16_t value) {
        if (address < 4096) {
            memory[address] = value;
        }
    }



        CPU::CPU(RAM* rawMemoryPointer){
            pmemory = rawMemoryPointer;
        }

        bool CPU::Status(){
            return run_flag;
        }
        // Main Step Function
       void CPU::Step(){
            uint16_t ins = pmemory->read(PC);
            uint16_t opcode = ins >> 12 ;
            uint16_t ins_address = ins & 0x0FFF; // 0000 0000 0000 0000
            switch (opcode)           
            {
            case HALT_:
                run_flag = false;
                break;
            case LOAD_:
                ACC = pmemory->read(ins_address);
                PC += 1;
                break;
            case STORE_:
                pmemory->write(ins_address, ACC);
                PC += 1;
                break;
            case ADD_:
                ACC= ALU(ACC, opcode, pmemory->read(ins_address),aeb);
                PC += 1;
                break;
            case SUB_:
                ACC= ALU(ACC, opcode, pmemory->read(ins_address),aeb);
                PC += 1;
                break;
            case INPUT_:
                std::cout << "INPUT:  >> ";
                std::cin >> ACC; // needs stricter input checking, has to be numeric, ok for now
                PC += 1;
                break;
            case OUTPUT_:
                std::cout << "OUTPUT: " << ACC << "\n";
                PC += 1;
                break;
            case JUMP_:
                PC = ins_address;
                break;
            case SKIPZERO_:
                if (ACC == 0) { 
                    PC += 2 ;
                    break;
                } 
                PC+= 1;
                break;
            case SKIPPOS_:
                if (ACC > 0) { 
                    PC += 2 ;
                    break;
                } 
                PC+= 1;
                break;
            case SKIPERR_:
                if (aeb) { 
                    PC += 2 ;
                    break;
                } 
                PC+= 1;
                break;
            case LOADI_:{
               uint16_t target_address = pmemory->read(ins_address); // must verify that there are no shenanigans when it comes to types
                ACC = pmemory->read(target_address);
                PC += 1;
                break;}
            case STOREI_:{
                uint16_t target_address = pmemory->read(ins_address);
                pmemory->write(target_address, ACC);
                PC += 1;
                break;}
            case SHIFT_:
                ACC = ALU(ACC,SHIFT_,pmemory->read(ins_address),aeb);
                PC += 1;
                break;
            case AND_:
                ACC = ALU(ACC,AND_ ,pmemory->read(ins_address),aeb);
                PC += 1;
                break;
            case XOR_:
                ACC = ALU(ACC, XOR_, pmemory->read(ins_address),aeb);
                PC += 1;
                break;
            default: // need to refine default logic
                std::cout << "UNKNOWN OPCODE: " << opcode << " PROGRAM HALTING";
                run_flag = false;
                break; 
            }
        



        }




    