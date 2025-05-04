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
#define ADDR_WIDTH 16 // Address width in bits
#define DATA_WIDTH 32 // Data width in bits


typedef sc_dt::sc_uint<ADDR_WIDTH> addr_t;
typedef sc_dt::sc_uint<DATA_WIDTH> data_t;

enum InstructionType {
    LOAD,
    STORE,
    XOR,
    AND,
    OR
};