#include "digital_signature_manager/digital_signature_manager.hpp"
#include "application_manager/application_manager.hpp"
#include "blockchain_storage/blockchain_storage.hpp"
#include "validator/validator.hpp"
#include "models/models.h"
#include "application_logic/application_logic.hpp"

int main(int argc, char **argv)
{
    BlockchainNode::Wallet wallet = BlockchainNode::DigitalSignatureManager::generate_wallet();

    BlockchainNode::ApplicationManager app_manager(5555);

    app_manager.set_on_new_block_received_callback(BlockchainNode::ApplicationLogic::on_new_block_receive);
    app_manager.set_on_new_transaction_received_callback(BlockchainNode::ApplicationLogic::on_new_transaction_receive);
    app_manager.start();
}
