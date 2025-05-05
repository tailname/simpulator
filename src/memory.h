#pragma once
#include "common.h"

SC_MODULE(Memory) {
 public:
    // Define the target socket
    simple_target_socket<Memory> proc_socket; 
    simple_target_socket<Memory> bench_socket;

    SC_CTOR(Memory) : proc_socket("proc_socket"), bench_socket("bench_socket") {
        memory.resize(MAX_MEMORY_SIZE, 0); 
        proc_socket.register_b_transport(this, &Memory::b_transport);
        bench_socket.register_b_transport(this, &Memory::b_transport);
    }


 private:
    std::vector<unsigned char> memory;
    void b_transport(tlm_generic_payload& trans, sc_time& delay);
    
};