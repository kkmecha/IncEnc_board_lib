#pragma once

#include "mbed.h"

class IncEnc_board {
public:
    IncEnc_board(int all_node_num);
    void encoder_calib_node(CAN &can, CANMessage &msg, int node);
    void encoder_calib_all(CAN &can, CANMessage &msg);
    void conv_data_node(CANMessage &msg, int64_t* angle, uint8_t node);
    void conv_data_all(CANMessage &msg, int64_t* angles);

private:
    int _all_node_num;
};