#include "IncEnc_board.h"

IncEnc_board::IncEnc_board(CAN &can, int all_node_num) : _can(can), _all_node_num(all_node_num){}

void IncEnc_board::encoder_reset_node(int node){
    CANMessage msg;
    msg.id   = 0x400 + node;
    msg.len  = 1;
    msg.data[0] = 0xff;
    _can.write(msg);
}

void IncEnc_board::encoder_reset_all(){
    for(int id = 1; id <= _all_node_num; id++){
        this->encoder_reset_node(id);
    }
}

void IncEnc_board::conv_data_node(int64_t *angle, uint8_t node){
    CANMessage msg;
    _can.read(msg);
    const int index = node - 1;
    if(msg.id == 0x400+node){
        angle[index] = 0;
        for (int i = 0; i < 8; i++) {
            angle[index] |= (int64_t)msg.data[i] << (8 * (7 - i));
        }
    }
}

void IncEnc_board::conv_data_all(int64_t *angles){
    for(int node = 1; node <= _all_node_num; node++) this->conv_data_node(angles, node);
}

