#ifndef __INCENC_BOARD_H__
#define __INCENC_BOARD_H__

#include "mbed.h"
#include <vector>
#include <map>

class IncEnc_board{
    public:
        IncEnc_board(CAN &can, int num, uint32_t start_id = 1);
        void read_all(int64_t* angles);
        bool reset_node(uint32_t node_id);

    private:
        void _rx_isr();

        CAN& _can;
        std::vector<uint32_t> _node_ids;
        std::map<uint32_t, int64_t> _angle_map;
};

#endif // __INCENC_BOARD_H__