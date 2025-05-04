#include <systemc.h> 
#include <iostream>
using namespace sc_core;
using namespace sc_dt;


SC_MODULE(Counter){
    sc_in<bool> clk;
    sc_in<bool> reset; 
    sc_in<bool> enable; 
    sc_out<sc_uint<4>> count; 
    
    sc_uint<4> count_value; 

    void Increment() {
        while(true) {
            wait(); 
            if(reset.read()) {
                count_value = 0; 
            } else if(enable.read()) {
                count_value++; 
            }
            count.write(count_value); 
        }
            
    }
    

    void PrintCount() {
        while(true) {
            wait(); 
            std::cout << "Count: " << count.read() << std::endl; 
        }
    }

    SC_CTOR(Counter) {
        SC_CTHREAD(Increment,clk.pos()); 
        
        

        SC_THREAD(PrintCount);
        sensitive << count;
    }
};


int sc_main(int argc, char* argv[]) {
   
    
    sc_signal<bool> clock;
    sc_signal<bool> reset; 
    sc_signal<bool> enable; 
    sc_signal<sc_uint<4>> count_value; 

    Counter counter("Counter"); 
    counter.clk(clock); 
    counter.reset(reset); 
    counter.enable(enable); 
    counter.count(count_value); 
    reset = 1;
    enable = 0;
    sc_start(0, SC_MS); 



    for (int i = 0; i < 10; i++) {
        clock = 0;
        sc_start(1, SC_MS); 
        clock = 1;
        sc_start(1, SC_MS); 
    }
    reset = 0; 
    enable = 1; 
    for (int i = 0; i < 10; i++) {
        clock = 0;
        sc_start(1, SC_MS); 
        clock = 1;
        sc_start(1, SC_MS); 
    }

   
    sc_stop(); 
    return 0; 
}