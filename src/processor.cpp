#include "processor.h"


void Processor::execute() {
    data_t current_instruction_word;
    cout<<"take clock"<<endl;
    while(true) {


        wait();
        if (!is_running)
            continue;

        mem_read(pc, current_instruction_word); // Read instruction from memory
        Instruction inst;
        inst.decode(current_instruction_word); // Decode the instruction

        pc += DATA_WIDTH; // Increment the program counter
        print_registers();
        cout <<" " << inst.opcode << " " << inst.reg_dst << " " << inst.reg_src << " " << inst.reg_src2 << endl; // Print the instruction
        switch (inst.opcode) {
            case LOAD:
                mem_read(inst.address, registers[inst.reg_dst]); // Load data from memory to register
                break;
            case STORE:
                mem_write(inst.address, registers[inst.reg_src]); // Store data from register to memory
                break;
            case ADD:
                registers[inst.reg_dst] = registers[inst.reg_src] + registers[inst.reg_src2]; // Add two registers and store the result in the destination register
                break;
            case HALT:
                is_running = false; // Stop execution
            default:
                SC_REPORT_ERROR("Processor", "Unknown instruction opcode");
                is_running = false; 
                break;
        }
        
    }
};

void Processor::handle_start() {
    registers.resize(REG_COUNT, 0); 

    if(reset_signal.read()) {
        is_running = false; // Stop execution on reset
        pc = 0;
        cout<<"take reset signal"<<endl;
        return;
    }
    cout<<"take start signal"<<endl;
    pc = start_address.read(); 
    is_running = true;     
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

void Processor::print_registers() {
    std::cout << "Registers: ";
    for (size_t i = 0; i < registers.size(); ++i) {
        std::cout << "R" << i << ": " << registers[i] << " ";
    }
    std::cout << std::endl;
}