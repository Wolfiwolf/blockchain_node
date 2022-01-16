#include "application_manager.hpp"
#include "../message_serializer/message_serializer.hpp"
#include "../debug/logger/logger.hpp"
#include "../storage/storage.hpp"
#include "../digital_signature_manager/digital_signature_manager.hpp"

namespace BlockchainNode
{
    NetworkCommunicator *NetworkCommunicator::_instance;

    NetworkCommunicator::NetworkCommunicator(int node_port)
        : _node_port(node_port),
          _on_new_block_received_callback(nullptr),
          _on_new_transaction_received_callback(nullptr)
    {
        _instance = this;
    }
    NetworkCommunicator::~NetworkCommunicator()
    {
    }

    NetworkCommunicator *NetworkCommunicator::get_instance()
    {
        return _instance;
    }

    void NetworkCommunicator::start()
    {
        if (_on_new_block_received_callback == nullptr)
            throw "on_new_block_received_callback is not set!";

        if (_on_new_transaction_received_callback == nullptr)
            throw "on_new_transaction_received_callback is not set!";

        _communicator.set_on_message_received_callback(on_message_received);
        _communicator.start_listening(_node_port);
    }

    void NetworkCommunicator::add_node(const BlockchainNodeContactInfo &nodeContactInfo)
    {
        _other_nodes.push_back(nodeContactInfo);
    }

    void NetworkCommunicator::set_on_new_block_received_callback(void (*on_new_block_received_callback)(BlockchainNode::NetworkCommunicator *app_manager, const BlockchainNode::Block &new_block, const std::string &sender_address))
    {
        _on_new_block_received_callback = on_new_block_received_callback;
    }

    void NetworkCommunicator::set_on_new_transaction_received_callback(void (*on_new_transaction_received_callback)(BlockchainNode::NetworkCommunicator *app_manager, const BlockchainNode::Transaction &new_transaction, const std::string &sender_address))
    {
        _on_new_transaction_received_callback = on_new_transaction_received_callback;
    }

    void NetworkCommunicator::broadcast_new_block(const Block &block)
    {
        for (const BlockchainNodeContactInfo &node : _other_nodes)
        {
            try
            {
                LOG_WNL("SENDING BLOCK");
                uint8_t data[4096];
                int num_of_bytes = MessageSerializer::block_to_bytes(block, data + 1);
                data[0] = 0x01;
                _communicator.send_to(node.ip_address.c_str(), node.port, (char *)data, num_of_bytes + 1);
            }
            catch (std::exception e)
            {
            }
        }
    }

    void NetworkCommunicator::broadcast_new_transaction(const Transaction &transaction)
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

    void NetworkCommunicator::on_message_received(uint8_t *data, int data_size)
    {
        NetworkCommunicator *app_manager = NetworkCommunicator::get_instance();
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