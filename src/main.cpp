#include "digital_signature_manager/digital_signature_manager.hpp"
#include "application_manager/application_manager.hpp"
#include "validator/validator.hpp"
#include "models/models.h"
#include "application_logic/application_logic.hpp"
#include "debug/logger/logger.hpp"
#include "rest_api_server/rest_api_server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3) 
    {
        LOG_WNL("Not enough arguments!");
        return 1;
    }

    int p2p_port = std::stoi(argv[1]);
    int api_port = std::stoi(argv[2]);

    BlockchainNode::Wallet wallet = BlockchainNode::DigitalSignatureManager::generate_wallet();

    BlockchainNode::RestApiServer apiServer;

    apiServer.start_server(api_port);

    BlockchainNode::ApplicationManager app_manager(p2p_port);

    app_manager.set_on_new_block_received_callback(BlockchainNode::ApplicationLogic::on_new_block_receive);
    app_manager.set_on_new_transaction_received_callback(BlockchainNode::ApplicationLogic::on_new_transaction_receive);
    app_manager.start();
}
