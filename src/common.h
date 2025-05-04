#pragma once
#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h> 
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include <iostream>
#include <vector>

using namespace sc_core;
using namespace sc_dt;
using namespace tlm;
using namespace tlm_utils;

#define MAX_MEMORY_SIZE 1024 // Memory size
