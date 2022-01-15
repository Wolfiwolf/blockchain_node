#include "rest_api_server.hpp"

#include <cpprest/http_listener.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "../debug/logger/logger.hpp"
#include "../validator/validator.hpp"
#include "../digital_signature_manager/digital_signature_manager.hpp"
#include "../json_operations/json_operations.hpp"
#include "../storage/storage.hpp"
#include "../application_manager/application_manager.hpp"
#include "../format_converter/format_converter.hpp"

using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;

namespace BlockchainNode
{

    RestApiServer::RestApiServer()
    {
    }

    RestApiServer::~RestApiServer()
    {
        delete _listener;
    }

    void RestApiServer::start_server(int listening_port)
    {
        std::string url = std::string("http://localhost:") + std::to_string(listening_port);

        _listener = new http_listener(url);
        _listener->support(methods::GET, handle_get_request);
        _listener->support(methods::POST, handle_post_request);
        _listener->open().wait();
    }

    void RestApiServer::handle_post_request(http_request request)
    {
        std::string path = request.request_uri().path();
        std::vector<std::string> path_parts = get_path_parts(path);

        if (path_parts[0] == "new-transaction")
        {
            long l;
            std::string str;
            while (true)
            {
                l = request.body().read().get();
                if (l == -1)
                    break;
                str += (char)l;
            }

            Transaction transaction = JsonOperations::get_transaction(str);

            on_new_transaction_request(&request, transaction);
        }
        else if (path_parts[0] == "new-node")
        {
            long l;
            std::string str;
            while (true)
            {
                l = request.body().read().get();
                if (l == -1)
                    break;
                str += (char)l;
            }

            BlockchainNodeContactInfo nodeInfo;
            nodeInfo.ip_address = JsonOperations::get_str_value(str, "ipAddress");
            nodeInfo.port = JsonOperations::get_int_value(str, "port");

            on_new_node_request(&request, nodeInfo);
        }
    }

    void RestApiServer::handle_get_request(http_request request)
    {
        std::string path = request.request_uri().path();

        std::vector<std::string> path_parts = get_path_parts(path);

        if (path_parts[0] == "unspent-transactions")
        {
            if (path_parts.size() == 2)
            {
                on_unspent_transactions_request(&request, path_parts[1]);
                return;
            }
        }

        request.reply(status_codes::NotFound);
    }

    void RestApiServer::on_unspent_transactions_request(http_request *request, const std::string &public_key)
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

    void RestApiServer::on_new_node_request(http_request *request, const BlockchainNodeContactInfo &nodeContactInfo)
    {
        ApplicationManager *app_manager = ApplicationManager::get_instance();
        app_manager->add_node(nodeContactInfo);
        request->reply(status_codes::OK);
    }

    void RestApiServer::on_new_transaction_request(http_request *request, const Transaction &transaction)
    {

        if (Validator::is_transaction_valid(transaction))
        {
            Storage::add_transaction(transaction);
            ApplicationManager *app_manager = ApplicationManager::get_instance();
            app_manager->broadcast_new_transaction(transaction);
            request->reply(status_codes::OK);

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

            Miner *miner = Miner::get_instance();
            if (!miner->is_minning())
                miner->start_mining_block(block);
        }
        else
        {
            request->reply(status_codes::NotAcceptable, "Transaction is not valid!");
        }
    }

    std::vector<std::string> RestApiServer::get_path_parts(const std::string &path)
    {
        std::vector<std::string> result;

        std::string current("");
        for (int i = 1; i < path.size(); ++i)
        {
            if (path[i] == '/')
            {
                result.push_back(current);
                current = "";
            }
            else
                current += path[i];
        }

        if (current != "")
            result.push_back(current);

        return result;
    }

}