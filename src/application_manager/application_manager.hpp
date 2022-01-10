#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>

#include "../models/models.h"
#include "../communicator/communicator.hpp"

namespace BlockchainNode
{

    class ApplicationManager
    {
    public:
        ApplicationManager(int node_port);
        ~ApplicationManager();

        void start();

        void set_on_new_block_received_callback(void (*on_new_block_received_callback)(BlockchainNode::ApplicationManager *app_manager, const BlockchainNode::Block &newBlock, const std::string &senderAddress));
        void set_on_new_transaction_received_callback(void (*on_new_transaction_received_callback)(BlockchainNode::ApplicationManager *app_manager, const BlockchainNode::Transaction &newTransaction, const std::string &senderAddress));

        static void broadcast_new_block(const Block &block);
        static void broadcast_new_transaction(const Transaction &transaction);

    private:
        Communicator _communicator;
        int _node_port;

        std::vector<BlockchainNodeContactInfo> _other_nodes;

        void (*_on_new_block_received_callback)(BlockchainNode::ApplicationManager *app_manager, const BlockchainNode::Block &new_block, const std::string &sender_address);
        void (*_on_new_transaction_received_callback)(BlockchainNode::ApplicationManager *app_manager, const BlockchainNode::Transaction &new_transaction, const std::string &sender_address);

        void on_message_received(uint8_t *data, int data_size);
    };

}

#endif
