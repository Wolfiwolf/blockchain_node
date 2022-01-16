#ifndef BLOCKCHAIN_STORAGE_H
#define BLOCKCHAIN_STORAGE_H

#include <vector>

#include "../models/models.h"

namespace BlockchainNode {

    class Storage
    {
    public:
        static void init();
        static void add_block(const Block &block);
        static void add_transaction(const Transaction &transaction);
        static void save_wallet(const Wallet &wallet);

        static bool is_block_in_storage(const Block &block);
        static bool is_transaction_in_storage(const Transaction &transaction);
        static std::vector<TxInWithAmount> get_public_key_unspent_transactions(const std::string &public_key);

        static const std::vector<Block> *get_blocks();
        static const std::vector<Transaction> *get_uncomfirmed_transactions();
        static const Wallet &get_wallet();

    private:
        static Wallet _wallet;
        static std::vector<Block> _blocks;
        static std::vector<Transaction> _uncomfirmed_transactions;

        static bool is_tx_out_unused(const std::string &transaction_hash, const TxOut &tx_out);
    };

}

#endif