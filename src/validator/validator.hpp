#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "../models/models.h"

namespace BlockchainNode
{

    class Validator
    {
    public:
        static bool is_block_valid(const Block &block);
        static bool is_transaction_valid(const Transaction &transaction);
    };

}

#endif