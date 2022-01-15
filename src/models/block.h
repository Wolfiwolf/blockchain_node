#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include "../models/transaction.h"

namespace BlockchainNode
{
    struct Block
    {
        int id;
        int timestamp;
        int nonce;
        int difficulty;
        std::string hash;
        std::string hash_of_previous_block;
        std::vector<Transaction> transactions;
    };

}

#endif