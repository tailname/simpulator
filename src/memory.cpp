#include "memory.h"


void Memory::b_transport(tlm_generic_payload& trans, sc_time& delay) {
    // Get the address and data from the transaction
    tlm_command cmd = trans.get_command();
    uint64 addr = trans.get_address();
    unsigned char* data = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();


    if (addr + len >= MAX_MEMORY_SIZE) {
        SC_REPORT_ERROR("Memory", "Address out of range");
    }

    if (cmd == tlm::TLM_READ_COMMAND) {
        // Read data from memory
        memcpy(data, &memory[addr], len);
    } else if (cmd == tlm::TLM_WRITE_COMMAND) {
        // Write data to memory
        memcpy(&memory[addr], data,len);
    } else {
        SC_REPORT_ERROR("Memory", "Invalid command");
    }
    
    delay += sc_time(10, SC_NS); // Simulate a delay
    trans.set_response_status(TLM_OK_RESPONSE);

};