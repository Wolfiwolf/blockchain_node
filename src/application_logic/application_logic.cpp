
#include "application_logic.hpp"

#include "../validator/validator.hpp"
#include "../storage/storage.hpp"
#include "../debug/logger/logger.hpp"
#include "../miner/miner.hpp"

namespace BlockchainNode
{
    namespace ApplicationLogic
    {

        void on_new_block_receive(
            BlockchainNode::ApplicationManager *app_manager,
            const BlockchainNode::Block &new_block,
            const std::string &sender_address)
        {
            bool res = Validator::is_block_valid(new_block);

            if (res)
                Storage::add_block(new_block);
        }

        void on_new_transaction_receive(
            BlockchainNode::ApplicationManager *app_manager,
            const BlockchainNode::Transaction &new_transaction,
            const std::string &sender_address)
        {
            if (!Validator::is_transaction_valid(new_transaction))
                return;

            if (!Storage::is_transaction_in_storage(new_transaction))
            {
                Storage::add_transaction(new_transaction);
                app_manager->broadcast_new_transaction(new_transaction);
            }

            const std::vector<BlockchainNode::Transaction> *transactions = BlockchainNode::Storage::get_uncomfirmed_transactions();
            if ((*transactions).size() < 3)
                return;

            const std::vector<BlockchainNode::Block> *blocks = BlockchainNode::Storage::get_blocks();

            BlockchainNode::Block block;
            block.id = (*blocks).back().id;
            block.nonce = 0;
            block.hash = "0x0";
            block.hash_of_previous_block = (*blocks).back().hash;
            block.difficulty = 3;

            BlockchainNode::Transaction transaction;

            for (const BlockchainNode::Transaction &transaction : *transactions)
            {
                block.transactions.push_back(transaction);
            }

            Miner *miner = Miner::get_instance();
            if (!miner->is_minning())
                miner->start_mining_block(block);
            
        }

    }
}