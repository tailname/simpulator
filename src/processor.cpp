#include "processor.h"


void Processor::execute() {
    while(is_running) {
        wait();

    }
};

void Processor::handle_start() {
    
    if (start_signal.read()) {
        
        pc = start_address.read(); // Read the start address from the input port
        registers.resize(REG_COUNT, 0); 
        is_running = true; 
        
    }
};

void Processor::mem_read(addr_t address, data_t& data) {
    tlm_generic_payload trans;
    sc_time delay = sc_time(0, SC_NS);
    unsigned char data_ptr[DATA_WIDTH / 8]; 

    trans.set_command(TLM_READ_COMMAND);
    trans.set_address(address);
    trans.set_data_ptr(data_ptr);
    trans.set_data_length(DATA_WIDTH / 8);
    trans.set_streaming_width(DATA_WIDTH / 8);
    trans.set_byte_enable_ptr(nullptr);
    trans.set_dmi_allowed(false);
    trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
    mem_socket->b_transport(trans, delay);

    if (trans.is_response_error()) {
        SC_REPORT_ERROR("Processor", "Memory read error");
        return;
    }
    
    for (int i = 0; i < DATA_WIDTH / 8; ++i) {
        data = (data << 8) | data_ptr[i]; // Big-endian conversion
    }
    wait(delay); 
};

void Processor::mem_write(addr_t address, data_t data) {
    tlm_generic_payload trans;
    sc_time delay = sc_time(0, SC_NS);
    unsigned char data_ptr[DATA_WIDTH / 8];
    for (int i = 0; i < DATA_WIDTH / 8; ++i) {
        data_ptr[i] = (data >> ((DATA_WIDTH / 8 - 1 - i) * 8)) & 0xFF; // Big-endian conversion
    }

    trans.set_command(tlm::TLM_WRITE_COMMAND);
    trans.set_address(address);
    trans.set_data_ptr(data_ptr);
    trans.set_data_length(DATA_WIDTH / 8);
    trans.set_streaming_width(DATA_WIDTH / 8);
    trans.set_byte_enable_ptr(nullptr);
    trans.set_dmi_allowed(false);
    trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    mem_socket->b_transport(trans, delay);

    if(trans.is_response_error()) {
        SC_REPORT_ERROR("Processor", "Memory write error");
        return;
    }
    wait(delay);

};