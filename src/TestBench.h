#pragma once
#include "common.h"
#include "memory.h"



SC_MODULE(TestBench) {
 public:
    simple_initiator_socket<TestBench> initiator_socket;
    
    SC_CTOR(TestBench) : initiator_socket("initiator_socket") {
        SC_THREAD(test_process);
    }
    
 private:
        void test_process();
};
    