#pragma once
#include "common.h"

SC_MODULE(Memory) {
 public:
    // Define the target socket
    tlm_utils::simple_target_socket<Memory> socket; 

    SC_CTOR(Memory) : socket("socket") {
        memory.resize(MAX_MEMORY_SIZE, 0); 
        socket.register_b_transport(this, &Memory::b_transport);
    }


 private:
    std::vector<unsigned char> memory;
    void b_transport(tlm_generic_payload& trans, sc_time& delay);
    
};