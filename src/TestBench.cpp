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
        return;
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
    wait(); // wait for reset to be asserted
    rst.write(false);
    std::cout << sc_time_stamp() << ": Deasserting reset." << std::endl;
    wait(); // wait for reset to be deasserted

    // 2. load program into memory
    addr_t prog_start_addr = 0x0; 
    load_program(prog_start_addr);

    // 3. send start signal to processor
    start_address.write(prog_start_addr);
    wait(); 
    std::cout << sc_time_stamp() << ": Sending start signal (address 0x" << std::hex << prog_start_addr << ")..." << std::endl;
    start_signal.write(true);
    wait();
    start_signal.write(false);

    // 4. wait for processor to finish execution
    std::cout << sc_time_stamp() << ": Testbench waiting for simulation end..." << std::endl;
    
    for(int i =0; i<1000; ++i) {
        wait();
    }
    //wait(5000, SC_NS); // wait for a while to let the processor execute

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
        cout<<hex<<encoded_inst;
        mem_write(start_addr + i * DATA_WIDTH, encoded_inst); // Write instruction to memory
    }
};

void TestBench::mem_write(addr_t address, data_t data) {
    tlm_generic_payload trans;
    sc_time delay = SC_ZERO_TIME;
    unsigned char data_ptr[DATA_WIDTH];
    memccpy(data_ptr, &data, 0, DATA_WIDTH); // Copy data to data

    trans.set_command(TLM_WRITE_COMMAND);
    trans.set_address(address);
    trans.set_data_ptr(data_ptr);
    trans.set_data_length(DATA_WIDTH);
    trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

    mem_socket->b_transport(trans, delay);
   
    if (trans.is_response_ok()) {
        std::cout << "Write successful at time " << sc_time_stamp() << std::endl;
    }
    else {
        std::cout << "Write failed at time " << sc_time_stamp() << std::endl;
    }
};



void TestBench::test_encoder(){
    if (DATA_WIDTH != 32 || INSTRUCTION_WIDTH != 8 || REG_WIDTH != 4 || ADDR_WIDTH != 16) {
        return;
    }
    Instruction inst1 = {LOAD, 0, 0, 0, 0x10};
    Instruction inst2 = {ADD, 1, 0, 2};
    Instruction inst3 = {STORE, 0, 1, 0, 0x20};
    Instruction inst4 = {HALT};

    data_t encoded_inst1 = encode_instruction(inst1);
    data_t encoded_inst2 = encode_instruction(inst2);
    data_t encoded_inst3 = encode_instruction(inst3);
    data_t encoded_inst4 = encode_instruction(inst4);

    assert(encoded_inst1 == 0b0000'0000'0000000000010000'00000000); // Check the encoding of LOAD instruction
    assert(encoded_inst2 == 0b0010'0001'0000'0010'00000000'00000000); // Check the encoding of ADD instruction
    assert(encoded_inst3 == 0b0001'0000000000100000'0001'00000000); // Check the encoding of STORE instruction
    assert(encoded_inst4 == 0b0011'00000000000000000000000000000); // Check the encoding of HALT instruction
}

void TestBench::test_decoder() {
    if (DATA_WIDTH != 32 || INSTRUCTION_WIDTH != 8 || REG_WIDTH != 4 || ADDR_WIDTH != 16) {
        return;
    }
    Instruction decoded_inst1;
    Instruction decoded_inst2;
    Instruction decoded_inst3;
    Instruction decoded_inst4;

    decoded_inst1.decode(0b0000'0000'0000000000010000'00000000);
    decoded_inst2.decode(0b0010'0001'0000'0010'00000000'00000000);
    decoded_inst3.decode(0b0001'0000000000100000'0001'00000000);
    decoded_inst4.decode(0b0011'00000000000000000000000000000);
    
    assert(decoded_inst1.opcode == LOAD && decoded_inst1.reg_dst == 0 && decoded_inst1.address == 0x10); // Check the decoding of LOAD instruction
    assert(decoded_inst2.opcode == ADD && decoded_inst2.reg_dst == 1 && decoded_inst2.reg_src == 2); // Check the decoding of ADD instruction
    assert(decoded_inst3.opcode == STORE && decoded_inst3.reg_src == 1 && decoded_inst3.address == 0x20); // Check the decoding of STORE instruction
    assert(decoded_inst4.opcode == HALT); // Check the decoding of HALT instruction
}