#include "common.h"
#include "memory.h"
#include "TestBench.h"
#include "processor.h"






int sc_main(int argc, char* argv[]) {
    Memory* memory;
    TestBench* test;
    Processor * processor;

    sc_signal<bool> clk;
    sc_signal<bool> rst;
    sc_signal<bool> start_signal;
    sc_signal<addr_t> start_address;


    memory = new Memory("Memory");
    test = new TestBench("TestBench");
    processor = new Processor("Processor");
        
    processor->clk(clk);
    
    processor->start_signal(start_signal);
    processor->reset_signal(rst);
    processor->start_address(start_address);
        
    test->rst(rst);
    test->clk(clk);
    test->start_signal(start_signal);
    test->start_address(start_address);

    test->mem_socket.bind(memory->bench_socket);
    processor->mem_socket.bind(memory->proc_socket);
    
    // start tasting memory
    //test->test_memory();
    // stop tasting memory  

    // start tasting encoder and decoder
    test->test_encoder();
    test->test_decoder();
    // stop tasting encoder and decoder

    // start tasting processor
    clk.write(0);
    start_signal.write(0);
    rst.write(0);

    for(int i = 0; i < 25000; ++i) {
        //std::cout << "Simulation time: " << sc_time_stamp() << std::endl;
        clk.write(!clk.read());
        sc_start(10, SC_NS); // Run for 10 ns
        
        
    }
    
    sc_stop(); 
    // stop tasting processor


    delete processor;
    delete memory;
    delete test;
    return 0; 
}