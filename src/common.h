#pragma once
#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h> 
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include <iostream>
#include <vector>

using namespace sc_core;
using namespace sc_dt;
using namespace tlm;
using namespace tlm_utils;

#define MAX_MEMORY_SIZE  1024 // Memory size
#define REG_COUNT 4 // Number of registers
#define REG_WIDTH 4 // Register width in bits
#define ADDR_WIDTH 16 // Address width in bits
#define DATA_WIDTH 32 // Data width in bits
#define INSTRUCTION_WIDTH 4 // Instruction width in bits



typedef sc_dt::sc_uint<ADDR_WIDTH> addr_t;
typedef sc_dt::sc_uint<DATA_WIDTH> data_t;

enum InstructionType {
    LOAD,
    STORE,
    ADD,
    HALT
};

struct Instruction {
    InstructionType opcode;
    sc_uint<INSTRUCTION_WIDTH> reg_dst;
    sc_uint<INSTRUCTION_WIDTH> reg_src;
    sc_uint<INSTRUCTION_WIDTH> reg_src2; // for ADD instruction
    addr_t address; // for LOAD and STORE instructions

    void decode(data_t raw_inst) {
        opcode   = static_cast<InstructionType>((raw_inst >> (DATA_WIDTH - INSTRUCTION_WIDTH)));
        reg_dst  = ((raw_inst << INSTRUCTION_WIDTH) >> (DATA_WIDTH - REG_WIDTH) ) & 0xF;
        reg_src  = ((raw_inst << (INSTRUCTION_WIDTH + REG_WIDTH)) >> (DATA_WIDTH - REG_WIDTH*2)) & 0xF;
        if (opcode == ADD) {
            reg_src2 = ((raw_inst << (INSTRUCTION_WIDTH + REG_WIDTH*2)) >> (DATA_WIDTH - REG_WIDTH*3)) & 0xF; // see the Redme
            address = 0; // default to 0 for ADD instruction
       } else {
            reg_src2 = 0; // default to 0 for LOAD and STORE instructions
            address  = (raw_inst << INSTRUCTION_WIDTH) >> (DATA_WIDTH - ADDR_WIDTH) & 0xFF; 
       }
    }
};