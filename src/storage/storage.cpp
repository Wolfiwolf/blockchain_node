#include "storage.hpp"

#include <string>
#include <cstring>
#include <cmath>
#include "../format_converter/format_converter.hpp"
#include "../models/models.h"
#include "../debug/logger/logger.hpp"
#include "../hasher/hasher.hpp"

namespace BlockchainNode
{
    std::vector<Block> Storage::_blocks;
    std::vector<Transaction> Storage::_uncomfirmed_transactions;

    void Storage::init()
    {
        Block block;
        block.id = 1;
        block.nonce = 0;
        block.hash = "0x0";
        block.hash_of_previous_block = "0x0";
        block.timestamp = 0;
        block.difficulty = 3;

        Transaction transaction;

        TxOut tx_out;
        tx_out.index = 0;
        tx_out.amount = static_cast<unsigned long int>(pow(10, 6) * pow(10, 12));
        tx_out.receiver_public_key = "0x031BE9DA01F032DFA7B065495F79750D10985D2E1DBD846A53D75198600BE61F4E";

        transaction.tx_outs.push_back(tx_out);

        transaction.hash = Hasher::hash_transaction(transaction);

        block.transactions.push_back(transaction);

        _blocks.push_back(block);
    }

    void Storage::add_block(const Block &block)
    {

        for (const Transaction &transaction : block.transactions)
        {
            for (int i = 0; i < _uncomfirmed_transactions.size(); ++i)
            {
                if (transaction.hash == _uncomfirmed_transactions[i].hash)
                {
                    _uncomfirmed_transactions.erase(_uncomfirmed_transactions.begin() + i);
                    break;
                }
            }
        }

        _blocks.push_back(block);
    }

    void Storage::add_transaction(const Transaction &transaction)
    {
        _uncomfirmed_transactions.push_back(transaction);
    }

    bool Storage::is_transaction_in_storage(const Transaction &transaction)
    {
        return false;
    }

    std::vector<TxInWithAmount> Storage::get_public_key_unspent_transactions(const std::string &public_key)
    {
        std::vector<TxInWithAmount> tx_ins;

        for (const Block &block : _blocks)
        {
            for (const Transaction &transaction : block.transactions)
            {
                for (const TxOut &tx_out : transaction.tx_outs)
                {
                    if (tx_out.receiver_public_key == public_key && is_tx_out_unused(transaction.hash, tx_out))
                    {
                        TxInWithAmount tx_in_with_amount;
                        TxIn tx_in;
                        tx_in.block_id = block.id;
                        tx_in.transaction_hash = transaction.hash;
                        tx_in.tx_out_index = tx_out.index;

                        tx_in_with_amount.amount = tx_out.amount;
                        tx_in_with_amount.tx_in = tx_in;

                        tx_ins.push_back(tx_in_with_amount);
                    }
                }
            }
        }

        return tx_ins;
    }

    const std::vector<Block> *Storage::get_blocks()
    {
        return &_blocks;
    }

    const std::vector<Transaction> *Storage::get_uncomfirmed_transactions()
    {
        return &_uncomfirmed_transactions;
    }

    bool Storage::is_tx_out_unused(const std::string &transaction_hash, const TxOut &tx_out)
    {
        for (const Block &block : _blocks)
        {
            for (const Transaction &transaction : block.transactions)
            {

                for (const TxIn &tx_in : transaction.tx_ins)
                {
                    if (tx_in.transaction_hash == transaction_hash && tx_in.tx_out_index == tx_out.index)
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }
}