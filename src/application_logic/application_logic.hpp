#ifndef APPLICATION_LOGIC_H
#define APPLICATION_LOGIC_H

#include "../models/models.h"
#include "../application_manager/application_manager.hpp"

namespace BlockchainNode
{
    namespace ApplicationLogic
    {

        void on_new_block_receive(
            BlockchainNode::ApplicationManager *app_manager,
            const BlockchainNode::Block &new_block,
            const std::string &sender_address);

        void on_new_transaction_receive(
            BlockchainNode::ApplicationManager *app_manager,
            const BlockchainNode::Transaction &new_transaction,
            const std::string &sender_address);

    }
}

#endif