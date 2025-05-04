#include "common.h"
#include "memory.h"
#include "TestBench.h"




SC_MODULE(Top){
    Memory* memory;
    TestBench* test;
    SC_CTOR(Top) {
        memory = new Memory("Memory");
        test = new TestBench("TestBench");
        test->initiator_socket.bind(memory->socket); 
    }
};


int sc_main(int argc, char* argv[]) {
   
    Top top("Top");
    sc_start(0, SC_MS); 
    sc_stop(); 
    return 0; 
}