#include "TestBench.h"

void TestBench::test_memory() {
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

    mem_socket->b_transport(trans, delay);
    
    if (trans.is_response_ok()) {
        std::cout << "Write successful at time " << sc_time_stamp() << std::endl;
    }

    // second test, read from address 0
    trans.set_command(TLM_READ_COMMAND);
    trans.set_address(0);
    trans.set_data_ptr(data);
    trans.set_data_length(4);
    trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

    mem_socket->b_transport(trans, delay);
    
    if (trans.is_response_ok()) {
        std::cout << "Read successful at time " << sc_time_stamp() 
                  << ", value = 0x" << std::hex << *(uint32_t*)data << std::endl;
    }

    // third test, read invalid address
    trans.set_command(TLM_READ_COMMAND);
    try {
        trans.set_address(MAX_MEMORY_SIZE + 1); // out of range address
        trans.set_data_length(4);
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

        mem_socket->b_transport(trans, delay);
    } catch (const std::exception& e) {
        std::cout << "The third test was completed successfully" << std::endl;
    }
    
    
    
    if (!trans.is_response_ok()) {
        std::cout << "Address error detected at time " << sc_time_stamp() << std::endl;
    }
};


void TestBench::test_processor() {
    std::cout << sc_time_stamp() << ": Testbench process starting..." << std::endl;

    // 1. reset processor 
    rst.write(true);
    start_signal.write(false);
    start_address.write(0);
    std::cout << sc_time_stamp() << ": Asserting reset..." << std::endl;
    wait(100, SC_NS); // wait for reset to be asserted
    rst.write(false);
    std::cout << sc_time_stamp() << ": Deasserting reset." << std::endl;
    wait(10, SC_NS); // wait for reset to be deasserted

    // 2. load program into memory
    addr_t prog_start_addr = 0x0; 
    load_program(prog_start_addr);

    // 3. send start signal to processor
    start_address.write(prog_start_addr);
    wait(1, SC_NS); 
    std::cout << sc_time_stamp() << ": Sending start signal (address 0x" << std::hex << prog_start_addr << ")..." << std::endl;
    start_signal.write(true);
    wait(10, SC_NS);
    start_signal.write(false);

    // 4. wait for processor to finish execution
    std::cout << sc_time_stamp() << ": Testbench waiting for simulation end..." << std::endl;
    wait(5000, SC_NS); // wait for a while to let the processor execute

    std::cout << sc_time_stamp() << ": Testbench finished. Stopping simulation." << std::endl;
    sc_stop(); // stop the simulation
};

// fucntion to encode instruction
data_t TestBench::encode_instruction(Instruction inst) {
    data_t encoded_inst = 0;
    encoded_inst |= (inst.opcode << (DATA_WIDTH - INSTRUCTION_WIDTH)); // opcode
    switch(inst.opcode) {
        case LOAD:
            encoded_inst |= (inst.reg_dst << (DATA_WIDTH - INSTRUCTION_WIDTH - REG_WIDTH)); // reg_dst
            encoded_inst |= (inst.address << (DATA_WIDTH - INSTRUCTION_WIDTH - REG_WIDTH - ADDR_WIDTH)); // address
            break;
        case STORE:
            encoded_inst |= (inst.address << (DATA_WIDTH - INSTRUCTION_WIDTH - ADDR_WIDTH)); // address
            encoded_inst |= (inst.reg_src << (DATA_WIDTH - INSTRUCTION_WIDTH - ADDR_WIDTH - REG_WIDTH)); // reg_src
            break;
        case ADD:
            encoded_inst |= (inst.reg_dst << (DATA_WIDTH - INSTRUCTION_WIDTH - REG_WIDTH)); // reg_dst
            encoded_inst |= (inst.reg_src << (DATA_WIDTH - INSTRUCTION_WIDTH - REG_WIDTH * 2)); // reg_src
            encoded_inst |= (inst.reg_src2 << (DATA_WIDTH - INSTRUCTION_WIDTH - REG_WIDTH * 3)); // reg_src2
            break;
        case HALT:
            break;
        default:
            std::cerr << "Unknown instruction opcode" << std::endl;
            break;
    }
    return encoded_inst;
};

// function to load program into memory
void TestBench::load_program(addr_t start_addr) {
    // Example program: LOAD, ADD, STORE, HALT
    program.push_back({LOAD, 0, 0, 0, 0x10}); // Load from address 0x10 to register 0
    program.push_back({ADD, 1, 0, 2}); // Add register 0 and register 2, store in register 1
    program.push_back({STORE, 1, 0, 0, 0x20}); // Store register 1 to address 0x20
    program.push_back({HALT}); // Halt the processor

    for (size_t i = 0; i < program.size(); ++i) {
        data_t encoded_inst = encode_instruction(program[i]);
        mem_write(start_addr + i * sizeof(data_t), encoded_inst); // Write instruction to memory
    }
};

void TestBench::mem_write(addr_t address, data_t data) {
    tlm_generic_payload trans;
    sc_time delay = SC_ZERO_TIME;
    unsigned char data_ptr[sizeof(data_t)];
    memccpy(data_ptr, &data, 0, sizeof(data_t)); // Copy data to data

    trans.set_command(TLM_WRITE_COMMAND);
    trans.set_address(address);
    trans.set_data_ptr(data_ptr);
    trans.set_data_length(sizeof(data_t));
    trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

    mem_socket->b_transport(trans, delay);

    if (trans.is_response_ok()) {
        std::cout << "Write successful at time " << sc_time_stamp() << std::endl;
    }
    else {
        std::cout << "Write failed at time " << sc_time_stamp() << std::endl;
    }
};



