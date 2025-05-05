#pragma once
#include "common.h"
#include "memory.h"



SC_MODULE(TestBench) {
 public:

    simple_initiator_socket<TestBench> initiator_socket;
    sc_out<bool> rst;
    sc_out<bool> start_signal;
    sc_out<addr_t> start_address;

    SC_CTOR(TestBench) : initiator_socket("initiator_socket") {
        SC_THREAD(test_memory);
    }
    
 private:
    void test_memory();
    void test_processor();
    void load_program(addr_t start_addr);
    data_t encode_instruction(Instruction inst);
    std::vector<Instruction> program; // Program to be loaded into memory
};
    