#include "rest_api_callbacks.hpp"

#include <cpprest/http_listener.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include "../debug/logger/logger.hpp"
#include "../validator/validator.hpp"
#include "../digital_signature_manager/digital_signature_manager.hpp"
#include "../json_operations/json_operations.hpp"
#include "../storage/storage.hpp"
#include "../application_manager/application_manager.hpp"
#include "../format_converter/format_converter.hpp"
#include "../config.hpp"
#include "../hasher/hasher.hpp"

using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;

namespace BlockchainNode
{
    namespace RestApiCallbacks
    {

        void on_unspent_transactions_request(http_request *request, const std::string &public_key)
        {
            std::vector<TxInWithAmount> tx_in_amounts = Storage::get_public_key_unspent_transactions(public_key);

            json::value postParameters = json::value::array();

            std::string json_str("[ ");

            int i = 0;
            for (const TxInWithAmount &tx_in : tx_in_amounts)
            {
                json_str += "{ \"txIn\": ";
                json_str += JsonOperations::tx_in_to_json(tx_in.tx_in);
                json_str += ", \"amount\": " + std::to_string(tx_in.amount);
                json_str += " }";

                if (++i != tx_in_amounts.size())
                {
                    json_str += ", ";
                }
            }

            json_str += " ]";

            request->reply(status_codes::OK, json_str);
        }

        void on_new_node_request(http_request *request, const BlockchainNodeContactInfo &nodeContactInfo)
        {
            NetworkCommunicator *network_communicator = NetworkCommunicator::get_instance();
            network_communicator->add_node(nodeContactInfo);
            request->reply(status_codes::OK);
        }

        void on_new_transaction_request(http_request *request, const Transaction &transaction)
        {

            if (Validator::is_transaction_valid(transaction))
            {
                Storage::add_transaction(transaction);
                NetworkCommunicator *network_communicator = NetworkCommunicator::get_instance();
                network_communicator->broadcast_new_transaction(transaction);
                request->reply(status_codes::OK);

                Miner *miner = Miner::get_instance();

                if (!miner->is_turned_on())
                    return;

                const std::vector<BlockchainNode::Transaction> *transactions = Storage::get_uncomfirmed_transactions();
                if ((*transactions).size() == 0)
                    return;

                int trans_counter = 0;
                std::vector<Transaction> transactions_for_block;
                for (const Transaction &transaction : *transactions)
                {
                    transactions_for_block.push_back(transaction);
                    trans_counter++;
                    if (trans_counter == CONFIG_MAX_TRANSACTIONS_PER_BLOCK)
                        break;
                }

                Block block = miner->transactions_to_block(transactions_for_block);

                if (!miner->is_minning())
                    miner->start_mining_block(block);
            }
            else
            {
                request->reply(status_codes::NotAcceptable, "Transaction is not valid!");
            }
        }

    }
}