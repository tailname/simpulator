#include "memory.h"
void Memory::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    // Get the address and data from the transaction
    tlm_command cmd = trans.get_command();
    uint64 addr = trans.get_address();
    unsigned char* data = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();


    if (addr + len >= SIZE) {
        SC_REPORT_ERROR("Memory", "Address out of range");
    }

    if (cmd == tlm::TLM_READ_COMMAND) {
        // Read data from memory
        memccpy(data, &memory[addr],0, len);
    } else if (cmd == tlm::TLM_WRITE_COMMAND) {
        // Write data to memory
        memccpy(&memory[addr], data,0, len);
    } else {
        SC_REPORT_ERROR("Memory", "Invalid command");
    }

    delay += sc_time(10, SC_NS); // Simulate a delay

}