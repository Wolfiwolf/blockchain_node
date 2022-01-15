#ifndef MINER_H
#define MINER_H

#include "../models/models.h"

namespace BlockchainNode
{

    class Miner
    {
    public:
        Miner();

        static Miner *get_instance();

        void start_mining_block(Block block);
        void stop_mining_block();
        
        void set_on_block_mined_callback(void (*on_block_mined_callback)(Miner *miner, Block block));

        bool is_minning();
    private:
        static Miner *_instance;
        bool _stop_mining_flag;
        bool _miner_running_flag;
        void (*_on_block_mined_callback)(Miner *miner, Block block);

        static void mine_block(Miner *miner, Block block, void (*on_block_mined_callback)(Miner *miner, Block block));
    };

}

#endif