#include "common.h"
#include "memory.h"
#include "TestBench.h"
#include "Processor.h"





SC_MODULE(Top){
    Memory* memory;
    TestBench* test;
    Processor * processor;


    sc_clock clk;
    sc_signal<bool> rst;
    sc_signal<bool> start_signal;
    sc_signal<addr_t> start_address;

    SC_CTOR(Top): clk("clk", 10, SC_NS) {
        memory = new Memory("Memory");
        test = new TestBench("TestBench");
        processor = new Processor("Processor");
        
        processor->clk(clk);
        processor->start_signal(start_signal);
        processor->reset_signal(rst);
        processor->start_address(start_address);
        
        test->rst(rst);
        test->start_signal(start_signal);
        test->start_address(start_address);

        test->mem_socket.bind(memory->socket);
        processor->mem_socket.bind(memory->socket);

    }

    ~Top() {
        delete processor;
        delete memory;
        delete test;
    }
};


int sc_main(int argc, char* argv[]) {
   
    Top top("Top");
    sc_start(0, SC_MS); 
    sc_stop(); 
    return 0; 
}