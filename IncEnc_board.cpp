#include "IncEnc_board.h"

#define CAN_RX_FLAG (1UL << 0)

IncEnc_board::IncEnc_board(CAN &can, int num, uint32_t start_id)
 : _can(can) {
    for (int i = 0; i < num; ++i) {
        uint32_t current_id = start_id + i;
        _node_ids.push_back(current_id);
        _angle_map[0x400 + current_id] = 0;
    }
    _can.attach(callback(this, &IncEnc_board::_rx_isr));
    _read_thread.start(callback(this, &IncEnc_board::_read_thread_func));
}

void IncEnc_board::read_all(int64_t* angles) {
    for (size_t i = 0; i < _node_ids.size(); ++i) {
        uint32_t can_id = 0x400 + _node_ids[i];
        angles[i] = _angle_map[can_id];
    }
}

void IncEnc_board::_rx_isr() {
    _event_flags.set(CAN_RX_FLAG);
}

void IncEnc_board::_read_thread_func() {
    while (true) {
        _event_flags.wait_any(CAN_RX_FLAG);

        CANMessage msg;
        while (_can.read(msg)) {
            if (_angle_map.count(msg.id)) {
                int64_t received_angle = 0;
                for (int i = 0; i < 8; ++i) {
                    received_angle |= ((int64_t)msg.data[i] << (8 * (7 - i)));
                }
                _angle_map[msg.id] = received_angle;
            }
        }
    }
}

bool IncEnc_board::reset_node(uint32_t node_id) {
    uint32_t can_id = 0x400 + node_id;
    const uint8_t reset_command[] = {0xff};
    CANMessage msg(can_id, reset_command, 1);
    return _can.write(msg);
}