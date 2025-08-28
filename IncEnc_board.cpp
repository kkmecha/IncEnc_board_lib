#include "IncEnc_board.h"

IncEnc_board::IncEnc_board(int all_node_num) : _all_node_num(all_node_num){}

void IncEnc_board::encoder_calib_node(CANMessage &msg, int node){
    msg.id = 0x400 + node;
    msg.data[0] = 0xff;
}

void IncEnc_board::encoder_calib_all(CANMessage &msg){
    for(int id = 1; id <= _all_node_num; id++){
        this->encoder_calib_node(msg, id);
    }
}

void IncEnc_board::conv_data_node(CANMessage &msg, int64_t *angle, uint8_t node){
    if(msg.id == 0x400 + node){
        for(int id = 1; id <= _all_node_num; id++){
            *angle |= ((int64_t)msg.data[id] >> (8 * (7 - id)));
        }
    }
}

void IncEnc_board::conv_data_all(CANMessage &msg, int64_t *angles){
    for(int node = 0; node < _all_node_num; node++) this->conv_data_node(msg, angles, node);
}

