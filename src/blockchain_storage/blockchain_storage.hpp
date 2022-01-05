#ifndef BLOCKCHAIN_STORAGE_H
#define BLOCKCHAIN_STORAGE_H

#include <vector>

#include "../models/models.h"

namespace BlockchainNode {

    class BlockchainStorage
    {
    public:
        BlockchainStorage();

        static void add_block(const Block &block);
        static void add_transaction(const Transaction &transaction);

    private:
        static std::vector<Block> _blocks;
        static std::vector<Transaction> _uncomfirmed_transaction;
    };

}

#endif