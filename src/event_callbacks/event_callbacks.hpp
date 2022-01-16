#ifndef EVENT_CALLBACKS_H
#define EVENT_CALLBACKS_H

#include "../models/models.h"
#include "../application_manager/application_manager.hpp"

namespace BlockchainNode
{
    namespace EventCallbacks
    {

        void on_new_block_receive(
            BlockchainNode::NetworkCommunicator *app_manager,
            const BlockchainNode::Block &new_block,
            const std::string &sender_address);

        void on_new_transaction_receive(
            BlockchainNode::NetworkCommunicator *app_manager,
            const BlockchainNode::Transaction &new_transaction,
            const std::string &sender_address);

        void on_block_mined(BlockchainNode::Miner *miner, BlockchainNode::Block mined_block);

    }
}

#endif