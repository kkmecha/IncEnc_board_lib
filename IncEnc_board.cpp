#include "mbed.h"
#include "IncEnc_board.h"
#include <vector>
#include <map>

IncEnc_board::IncEnc_board(CAN &can, int num, uint32_t start_id = 1) : _can(can){
    // 指定された個数と開始IDから、管理対象のIDリストを自動生成する
    for (int i = 0; i < _num; ++i) {
        uint32_t current_id = start_id + i;
        _node_ids.push_back(current_id);
        // マップを初期化
        _angle_map[0x400 + current_id] = 0;
    }
    _can.attach(callback(this, &IncEnc_board::_rx_isr));
 }

void IncEnc_board::read_all(int64_t* angle) {
    for (size_t i = 0; i < _node_ids.size(); ++i) {
        uint32_t can_id = 0x400 + _node_ids[i];
        angle[i] = _angle_map[can_id];
    }
}

bool IncEnc_board::reset_node(uint32_t node_id) {
    uint32_t can_id = 0x400 + node_id;
    const uint8_t reset_command[] = {0xff};
    CANMessage msg(can_id, reset_command);
    return _can.write(msg);
}

void IncEnc_board::_rx_isr() {
    CANMessage msg;
    if (_can.read(msg)) {
        if (_angle_map.count(msg.id)) {
            int64_t received_angle = 0;
            for (int i = 0; i < 8; ++i) {
                received_angle |= ((int64_t)msg.data[i] << (8 * (7 - i)));
            }
            _angle_map[msg.id] = received_angle;
        }
    }
}