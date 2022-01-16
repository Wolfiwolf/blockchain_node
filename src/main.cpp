#include <chrono>

#include "digital_signature_manager/digital_signature_manager.hpp"
#include "application_manager/application_manager.hpp"
#include "validator/validator.hpp"
#include "models/models.h"
#include "event_callbacks/event_callbacks.hpp"
#include "debug/logger/logger.hpp"
#include "rest_api_server/rest_api_server.hpp"
#include "miner/miner.hpp"
#include "storage/storage.hpp"
#include "format_converter/format_converter.hpp"
#include "config.hpp"
#include "hasher/hasher.hpp"
#include "rest_api_callbacks/rest_api_callbacks.hpp"

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        LOG_WNL("Not enough arguments!");
        return 1;
    }

    int p2p_port = std::stoi(argv[1]);
    int api_port = std::stoi(argv[2]);
    bool node_is_miner = std::stoi(argv[3]) == 1;

    LOG_WNL("Http API for wallets running on: " << api_port);
    LOG_WNL("TCP/IP API for nodes running on: " << p2p_port);

    BlockchainNode::Wallet wallet = BlockchainNode::DigitalSignatureManager::generate_wallet();
    BlockchainNode::Storage::init();
    BlockchainNode::Storage::save_wallet(wallet);

    BlockchainNode::Miner miner;
    miner.set_on_block_mined_callback(BlockchainNode::EventCallbacks::on_block_mined);
    if (node_is_miner)
        miner.turn_on();

    BlockchainNode::RestApiServer api_server;
    api_server.set_on_new_node_request_callback(BlockchainNode::RestApiCallbacks::on_new_node_request);
    api_server.set_on_unspent_transactions_request_callback(BlockchainNode::RestApiCallbacks::on_unspent_transactions_request);
    api_server.set_on_new_transaction_request_callback(BlockchainNode::RestApiCallbacks::on_new_transaction_request);
    api_server.start_server(api_port);

    BlockchainNode::NetworkCommunicator network_communicator(p2p_port);
    network_communicator.set_on_new_block_received_callback(BlockchainNode::EventCallbacks::on_new_block_receive);
    network_communicator.set_on_new_transaction_received_callback(BlockchainNode::EventCallbacks::on_new_transaction_receive);
    network_communicator.start();
}
