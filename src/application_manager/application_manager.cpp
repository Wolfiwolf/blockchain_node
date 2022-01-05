#include "application_manager.hpp"
#include "../message_serializer/message_serializer.hpp"

namespace BlockchainNode
{

    ApplicationManager::ApplicationManager(int node_port)
        : _node_port(node_port),
          _on_new_block_received_callback(nullptr),
          _on_new_transaction_received_callback(nullptr)
    {
    }
    ApplicationManager::~ApplicationManager()
    {
    }

    void ApplicationManager::start()
    {
        if (_on_new_block_received_callback == nullptr)
            throw "on_new_block_received_callback is not set!";

        if (_on_new_transaction_received_callback == nullptr)
            throw "on_new_transaction_received_callback is not set!";

        _communicator.start_listening(_node_port);
    }

    void ApplicationManager::set_on_new_block_received_callback(void (*on_new_block_received_callback)(const BlockchainNode::Block &new_block, const std::string &sender_address))
    {
        _on_new_block_received_callback = on_new_block_received_callback;
    }

    void ApplicationManager::set_on_new_transaction_received_callback(void (*on_new_transaction_received_callback)(const BlockchainNode::Transaction &new_transaction, const std::string &sender_address))
    {
        _on_new_transaction_received_callback = on_new_transaction_received_callback;
    }

    void ApplicationManager::broadcast(const std::string &msg)
    {
    }

    void ApplicationManager::broadcast_except(const std::string &msg, const std::string &address)
    {
    }

    void ApplicationManager::on_message_received(uint8_t *data, int data_size)
    {
        if (data[0] == 0)
        {
            Transaction transaction = MessageSerializer::bytes_to_transaction(data + 1);
            _on_new_transaction_received_callback(transaction, "");
        }
        else if (data[0] == 0x01)
        {
            Block block = MessageSerializer::bytes_to_block(data + 1);
            _on_new_block_received_callback(block, "");
        }
    }
}