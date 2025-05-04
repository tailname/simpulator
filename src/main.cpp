#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h> 
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <iostream>

using namespace sc_core;
using namespace sc_dt;

class Memory : public sc_module {
 public:
    tlm_utils::simple_target_socket<Memory> socket; 
    SC_CTOR(Memory) : socket("socket") {
    }

    
};
class Initiator : public sc_module {
 public:
    tlm_utils::simple_initiator_socket<Initiator> socket; 
    SC_CTOR(Initiator) : socket("socket") {
    }
};

SC_MODULE(Top){
    Memory* memory;
    Initiator* initiator;
    SC_CTOR(Top){
        memory = new Memory("Memory");
        initiator = new Initiator("Initiator");
        initiator->socket.bind(memory->socket); 
    }
};


int sc_main(int argc, char* argv[]) {
   
    
    
    



    Top top("Top");
    sc_start(0, SC_MS); 
    //sc_start(1, SC_MS);
   
    sc_stop(); 
    return 0; 
}