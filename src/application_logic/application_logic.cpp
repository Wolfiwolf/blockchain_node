
#include "application_logic.hpp"

#include "../validator/validator.hpp"
#include "../blockchain_storage/blockchain_storage.hpp"

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
                BlockchainStorage::add_block(new_block);
        }

        void on_new_transaction_receive(
            BlockchainNode::ApplicationManager *app_manager,
            const BlockchainNode::Transaction &new_transaction,
            const std::string &sender_address)
        {
            bool res = Validator::is_transaction_valid(new_transaction);

            if (res)
                BlockchainStorage::add_transaction(new_transaction);
        }

    }
}