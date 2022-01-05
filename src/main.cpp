#include "digital_signature_manager/digital_signature_manager.hpp"
#include "application_manager/application_manager.hpp"
#include "blockchain_storage/blockchain_storage.hpp"
#include "validator/validator.hpp"
#include "models/models.h"

void on_new_block_receive(const BlockchainNode::Block &newBlock, const std::string &senderAddress);
void on_new_transaction_receive(const BlockchainNode::Transaction &newTransaction, const std::string &senderAddress);

int main(int argc, char **argv)
{
    BlockchainNode::Wallet wallet = BlockchainNode::DigitalSignatureManager::generate_wallet();

    BlockchainNode::ApplicationManager app_manager(5555);

    app_manager.set_on_new_block_received_callback(on_new_block_receive);
    app_manager.set_on_new_transaction_received_callback(on_new_transaction_receive);
    app_manager.start();
}

void on_new_block_receive(BlockchainNode::ApplicationManager *app_manager, const BlockchainNode::Block &new_block, const std::string &sender_address)
{
    bool res = BlockchainNode::Validator::is_block_valid(new_block);

    if (res)
        BlockchainNode::BlockchainStorage::add_block(new_block);
}

void on_new_transaction_receive(BlockchainNode::ApplicationManager *app_manager, const BlockchainNode::Transaction &new_transaction, const std::string &sender_address)
{
    bool res = BlockchainNode::Validator::is_transaction_valid(new_transaction);

    if (res)
        BlockchainNode::BlockchainStorage::add_transaction(new_transaction);
}
