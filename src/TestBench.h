#pragma once
#include "common.h"
#include "memory.h"



SC_MODULE(TestBench) {
 public:
    void test_encoder();
    void test_decoder();
    void test_memory();
    simple_initiator_socket<TestBench> mem_socket;
    
    sc_out<bool> reset_signal;
    sc_out<bool> start_signal;
    sc_out<addr_t> start_address;
    sc_in<bool> clk;

    SC_CTOR(TestBench) : mem_socket("mem_socket") {
        //SC_THREAD(test_memory);
        
        SC_THREAD(test_processor);
        sensitive << clk.pos();
    }
    
 private:
    
    void test_processor();
    void load_program(addr_t start_addr);
    void store_program();
    data_t encode_instruction(Instruction inst);
    void mem_write(addr_t address, data_t data);
    std::vector<Instruction> program; // Program to be loaded into memory
};
    