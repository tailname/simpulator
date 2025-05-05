#pragma once
#include "common.h"
#include "memory.h"



SC_MODULE(TestBench) {
 public:

    simple_initiator_socket<TestBench> mem_socket;
    
    sc_out<bool> rst;
    sc_out<bool> start_signal;
    sc_out<addr_t> start_address;

    SC_CTOR(TestBench) : mem_socket("mem_socket") {
        SC_THREAD(test_memory);
    }
    
 private:
    void test_memory();
    void test_processor();
    void load_program(addr_t start_addr);
    data_t encode_instruction(Instruction inst);
    void mem_write(addr_t address, data_t data);
    std::vector<Instruction> program; // Program to be loaded into memory
};
    