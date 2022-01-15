
#include "application_logic.hpp"

#include "../validator/validator.hpp"
#include "../storage/storage.hpp"
#include "../debug/logger/logger.hpp"

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
            LOG_WNL("TRANSACTION:");
            LOG_WNL(new_transaction.timestamp);
            LOG_WNL(new_transaction.sender_signature);
            LOG_WNL(new_transaction.sender_public_key);
            LOG_WNL(new_transaction.hash);
            LOG_WNL(new_transaction.gas);
            LOG_WNL("TXINS:");
            for (const TxIn &tx_in : new_transaction.tx_ins)
            {
                LOG_WNL(tx_in.block_id);
                LOG_WNL(tx_in.transaction_hash);
                LOG_WNL(tx_in.tx_out_index);
            }

            LOG_WNL("TXOUTS:");
            for (const TxOut &tx_out : new_transaction.tx_outs)
            {
                LOG_WNL(tx_out.index);
                LOG_WNL(tx_out.amount);
                LOG_WNL(tx_out.receiver_public_key);
            }

            if (!Validator::is_transaction_valid(new_transaction))
            {
                LOG_WNL("TRANSACTION IS NOT VALID!");
                return;
            }

            LOG_WNL("TRANSACTION IS VALID!");

            if (!Storage::is_transaction_in_storage(new_transaction))
            {
                Storage::add_transaction(new_transaction);
                app_manager->broadcast_new_transaction(new_transaction);
            }

            const std::vector<Transaction> *transactions = Storage::get_uncomfirmed_transactions();
        }

    }
}