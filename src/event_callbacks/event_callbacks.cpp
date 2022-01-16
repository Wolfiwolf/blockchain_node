#include "event_callbacks.hpp"

#include "../validator/validator.hpp"
#include "../storage/storage.hpp"
#include "../debug/logger/logger.hpp"
#include "../miner/miner.hpp"
#include "../config.hpp"
#include "../hasher/hasher.hpp"

namespace BlockchainNode
{
    namespace EventCallbacks
    {

        void on_new_block_receive(
            BlockchainNode::NetworkCommunicator *app_manager,
            const BlockchainNode::Block &new_block,
            const std::string &sender_address)
        {
            if (Storage::is_block_in_storage(new_block))
                return;

            if (Validator::is_block_valid(new_block))
            {
                Storage::add_block(new_block);
                app_manager->broadcast_new_block(new_block);
            }
        }

        void on_new_transaction_receive(
            BlockchainNode::NetworkCommunicator *app_manager,
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

            Miner *miner = Miner::get_instance();
            if (!miner->is_turned_on())
                return;

            const std::vector<BlockchainNode::Transaction> *transactions = BlockchainNode::Storage::get_uncomfirmed_transactions();
            if ((*transactions).size() == 0)
                return;

            int trans_counter = 0;
            std::vector<Transaction> transactions_for_block;
            for (const Transaction &transaction : *transactions)
            {
                transactions_for_block.push_back(transaction);
                trans_counter++;
                if (trans_counter == CONFIG_MAX_TRANSACTIONS_PER_BLOCK)
                    break;
            }

            BlockchainNode::Block block = miner->transactions_to_block(transactions_for_block);

            if (!miner->is_minning())
                miner->start_mining_block(block);
        }

        void on_block_mined(BlockchainNode::Miner *miner, BlockchainNode::Block mined_block)
        {
            BlockchainNode::Storage::add_block(mined_block);
            BlockchainNode::NetworkCommunicator *app_manager = BlockchainNode::NetworkCommunicator::get_instance();
            app_manager->broadcast_new_block(mined_block);

            const std::vector<BlockchainNode::Transaction> *transactions = BlockchainNode::Storage::get_uncomfirmed_transactions();
            if ((*transactions).size() == 0)
                return;

            int trans_counter = 0;
            std::vector<Transaction> transactions_for_block;
            for (const Transaction &transaction : *transactions)
            {
                transactions_for_block.push_back(transaction);
                trans_counter++;
                if (trans_counter == CONFIG_MAX_TRANSACTIONS_PER_BLOCK)
                    break;
            }

            BlockchainNode::Block block = miner->transactions_to_block(transactions_for_block);

            if (!miner->is_minning())
                miner->start_mining_block(block);
        }

    }
}