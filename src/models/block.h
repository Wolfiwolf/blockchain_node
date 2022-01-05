#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include "../models/transaction.h"

namespace BlockchainNode
{
    struct Block
    {
        int id;
        int nonce;
        std::string hash;
        std::string hash_of_previous_block;
        int reward_to_miner;
        int timestamp;
        Transaction transactions[3];
        uint8_t minerPublicKey[32];
    };

}

#endif