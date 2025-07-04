#include"common.h"
#include"memory.h"

SC_MODULE(Processor) {
 public:
    sc_in<bool> clk;
    sc_in<bool> start_signal;
    sc_in<bool> reset_signal;
    sc_in<addr_t> start_address;
    simple_initiator_socket<Processor> mem_socket;

    SC_CTOR(Processor): mem_socket("mem_socket") {
        registers.resize(REG_COUNT, 0); // Initialize registers to 0
        
        SC_THREAD(execute);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(handle_start);
        sensitive << start_signal.pos() << reset_signal.pos();
        dont_initialize();
    }
    void print_registers();
 private:
 
    void execute();
    void handle_start(); 
    void mem_read(addr_t address, data_t& data);
    void mem_write(addr_t address, data_t data);
    addr_t pc;
    std::vector<data_t> registers;
    bool is_running = false;  
    
     
};