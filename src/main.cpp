#include "digital_signature_manager/digital_signature_manager.hpp"
#include "application_manager/application_manager.hpp"
#include "validator/validator.hpp"
#include "models/models.h"
#include "application_logic/application_logic.hpp"
#include "debug/logger/logger.hpp"
#include "rest_api_server/rest_api_server.hpp"
#include "miner/miner.hpp"
#include "storage/storage.hpp"

void on_block_mined(BlockchainNode::Miner *miner, BlockchainNode::Block mined_block)
{
    BlockchainNode::Storage::add_block(mined_block);

    const std::vector<BlockchainNode::Transaction> *transactions = BlockchainNode::Storage::get_uncomfirmed_transactions();
    if ((*transactions).size() < 3)
        return;

    const std::vector<BlockchainNode::Block> *blocks = BlockchainNode::Storage::get_blocks();

    BlockchainNode::Block block;
    block.id = (*blocks).back().id;
    block.nonce = 0;
    block.hash = "0x0";
    block.hash_of_previous_block = (*blocks).back().hash;
    block.difficulty = 3;

    BlockchainNode::Transaction transaction;

    for (const BlockchainNode::Transaction &transaction : *transactions)
    {
        block.transactions.push_back(transaction);
    }

    miner->start_mining_block(block);
}

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

    BlockchainNode::Wallet wallet = BlockchainNode::DigitalSignatureManager::generate_wallet();

    BlockchainNode::RestApiServer apiServer;

    apiServer.start_server(api_port);

    BlockchainNode::Miner miner;

    miner.set_on_block_mined_callback(on_block_mined);

    BlockchainNode::ApplicationManager app_manager(p2p_port);

    miner.start_mining_block((*BlockchainNode::Storage::get_blocks())[0]);

    app_manager.set_on_new_block_received_callback(BlockchainNode::ApplicationLogic::on_new_block_receive);
    app_manager.set_on_new_transaction_received_callback(BlockchainNode::ApplicationLogic::on_new_transaction_receive);

    LOG_WNL("Api for wallets running on: " << api_port);
    LOG_WNL("Api for nodes running on: " << api_port);

    app_manager.start();
}
