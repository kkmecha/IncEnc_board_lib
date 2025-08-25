#pragma once

#include "mbed.h"
#include <vector>
#include <map>

class IncEnc_board {
public:
    IncEnc_board(CAN &can, int num, uint32_t start_id = 1);
    void read_all(int64_t* angles);
    bool reset_node(uint32_t node_id);

private:
    void _rx_isr(); 
    void _read_thread_func();

    CAN& _can;
    std::vector<uint32_t> _node_ids;
    std::map<uint32_t, int64_t> _angle_map;

    Thread _read_thread; 
    EventFlags _event_flags; 
};