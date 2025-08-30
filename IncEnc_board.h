#pragma once

#include "mbed.h"

class IncEnc_board {
public:
    IncEnc_board(CAN &can, int all_node_num);
    void encoder_reset_node(int node);
    void encoder_reset_all();
    void conv_data_node(int64_t* angle, uint8_t node);
    void conv_data_all(int64_t* angles);

private:
    int _all_node_num;
    CAN &_can;
};
