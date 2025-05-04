#include "processor.h"


void Processor::execute() {};

void Processor::handle_start() {
    
    if (start_signal.read()) {
        
        pc =  start_address.read(); // Read the start address from the input port
        registers[0] = 0; 
        
    }
}