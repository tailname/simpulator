#include "TestBench.h"

void TestBench::test_process() {
    tlm_generic_payload trans;
    sc_time delay = SC_ZERO_TIME;
    unsigned char data[4];

    // first test, write 0xAAAA to address 0
    trans.set_command(TLM_WRITE_COMMAND);
    trans.set_address(0);
    *(uint32_t*)data = 0xAAAAA;
    trans.set_data_ptr(data);
    trans.set_data_length(4);
    trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

    initiator_socket->b_transport(trans, delay);
    
    if (trans.is_response_ok()) {
        std::cout << "Write successful at time " << sc_time_stamp() << std::endl;
    }

    // second test, read from address 0
    trans.set_command(TLM_READ_COMMAND);
    trans.set_address(0);
    trans.set_data_ptr(data);
    trans.set_data_length(4);
    trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

    initiator_socket->b_transport(trans, delay);
    
    if (trans.is_response_ok()) {
        std::cout << "Read successful at time " << sc_time_stamp() 
                  << ", value = 0x" << std::hex << *(uint32_t*)data << std::endl;
    }

    // third test, read invalid address
    trans.set_command(TLM_READ_COMMAND);
    trans.set_address(MAX_MEMORY_SIZE + 1); // out of range address
    trans.set_data_length(4);
    trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

    initiator_socket->b_transport(trans, delay);
    
    if (!trans.is_response_ok()) {
        std::cout << "Address error detected at time " << sc_time_stamp() << std::endl;
    }
}