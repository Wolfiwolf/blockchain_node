#include "application_manager.hpp"
#include "../message_serializer/message_serializer.hpp"
#include "../debug/logger/logger.hpp"
#include "../storage/storage.hpp"

namespace BlockchainNode
{
    ApplicationManager *ApplicationManager::_instance;

    ApplicationManager::ApplicationManager(int node_port)
        : _node_port(node_port),
          _on_new_block_received_callback(nullptr),
          _on_new_transaction_received_callback(nullptr)
    {
        _instance = this;
        Storage::init();
    }
    ApplicationManager::~ApplicationManager()
    {
    }

    ApplicationManager *ApplicationManager::get_instance()
    {
        return _instance;
    }

    void ApplicationManager::start()
    {
        if (_on_new_block_received_callback == nullptr)
            throw "on_new_block_received_callback is not set!";

        if (_on_new_transaction_received_callback == nullptr)
            throw "on_new_transaction_received_callback is not set!";


        _communicator.set_on_message_received_callback(on_message_received);
        _communicator.start_listening(_node_port);
    }

    void ApplicationManager::add_node(const BlockchainNodeContactInfo &nodeContactInfo)
    {
        _other_nodes.push_back(nodeContactInfo);
    }

    void ApplicationManager::set_on_new_block_received_callback(void (*on_new_block_received_callback)(BlockchainNode::ApplicationManager *app_manager, const BlockchainNode::Block &new_block, const std::string &sender_address))
    {
        _on_new_block_received_callback = on_new_block_received_callback;
    }

    void ApplicationManager::set_on_new_transaction_received_callback(void (*on_new_transaction_received_callback)(BlockchainNode::ApplicationManager *app_manager, const BlockchainNode::Transaction &new_transaction, const std::string &sender_address))
    {
        _on_new_transaction_received_callback = on_new_transaction_received_callback;
    }

    void ApplicationManager::broadcast_new_block(const Block &block)
    {
    }

    void ApplicationManager::broadcast_new_transaction(const Transaction &transaction)
    {
        for (const BlockchainNodeContactInfo &node : _other_nodes)
        {
            try
            {
                uint8_t data[1028];
                int num_of_bytes = MessageSerializer::transaction_to_bytes(transaction, data + 1);
                data[0] = 0;
                _communicator.send_to(node.ip_address.c_str(), node.port, (char *)data, num_of_bytes + 1);
            }
            catch (std::exception e)
            {
            }
        }
    }

    void ApplicationManager::on_message_received(uint8_t *data, int data_size)
    {
        ApplicationManager *app_manager = ApplicationManager::get_instance();
        if (data[0] == 0)
        {
            Transaction transaction = MessageSerializer::bytes_to_transaction(data + 1);
            app_manager->_on_new_transaction_received_callback(app_manager, transaction, "");
        }
        else if (data[0] == 0x01)
        {
            Block block = MessageSerializer::bytes_to_block(data + 1);
            app_manager->_on_new_block_received_callback(app_manager, block, "");
        }
    }
}