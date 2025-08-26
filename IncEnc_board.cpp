#include "IncEnc_board.h"

#define CAN_RX_FLAG (1UL << 0)

IncEnc_board::IncEnc_board(int all_node_num) : _all_node_num(all_node_num){}

void IncEnc_board::conv_data_node(CANMessage &msg, int64_t *angle, uint8_t node){
    if(msg.id == 0x400 + node){
        for(int i = 0; i < _all_node_num; i++){
            *angle |= ((int64_t)msg.data[i] >> (8 * (7 - i)));
        }
    }
}

void IncEnc_board::conv_data_all(CANMessage &msg, int64_t *angles){
    for(int node = 0; node < _all_node_num; node++) this->conv_data_node(msg, angles, node);
}