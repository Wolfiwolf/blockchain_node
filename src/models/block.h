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
        uint8_t hash[32];
        uint8_t hash_of_previous_block[32];
        int reward_to_miner;
        int timestamp;
        Transaction transactions[3];
        uint8_t minerPublicKey[32];
    };

}

#endif