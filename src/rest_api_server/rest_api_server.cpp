#include "rest_api_server.hpp"

#include <cpprest/http_listener.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "../debug/logger/logger.hpp"
#include "../validator/validator.hpp"
#include "../digital_signature_manager/digital_signature_manager.hpp"
#include "../json_operations/json_operations.hpp"
#include "../blockchain_storage/blockchain_storage.hpp"
#include "../application_manager/application_manager.hpp"

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

        LOG_WNL("REST API running on " << url);
    }

    void RestApiServer::handle_post_request(http_request request)
    {
        std::string path = request.request_uri().path();
        std::vector<std::string> path_parts = get_path_parts(path);

        LOG_WNL(path);
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

            TransactionWeb transaction;
            transaction.timestamp = JsonOperations::get_int_value(str, "timestamp");
            transaction.sender_signature = JsonOperations::get_str_value(str, "senderSignature");
            transaction.sender_signature_len = JsonOperations::get_int_value(str, "senderSignatureLen");
            transaction.hash = JsonOperations::get_str_value(str, "hash");
            transaction.sender_public_key = JsonOperations::get_str_value(str, "senderPublicKey");
            transaction.receiver_public_key = JsonOperations::get_str_value(str, "receiverPublicKey");
            transaction.amount = JsonOperations::get_int_value(str, "amount");
            transaction.gas = JsonOperations::get_int_value(str, "gas");

            on_new_transaction_request(&request, transaction);
        }
    }

    void RestApiServer::handle_get_request(http_request request)
    {
        std::string path = request.request_uri().path();

        std::vector<std::string> path_parts = get_path_parts(path);

        if (path_parts[0] == "balance-of")
        {
            if (path_parts.size() == 2)
            {
                on_balance_of_request(&request, path_parts[1]);
                return;
            }
        }

        request.reply(status_codes::NotFound);
    }

    void RestApiServer::on_balance_of_request(http_request *request, const std::string &address)
    {
        request->reply(status_codes::OK, "1000000");
    }

    void RestApiServer::on_new_transaction_request(http_request *request, const TransactionWeb &transactionWeb)
    {
        LOG_WNL("Time stamp:");
        LOG_WNL(transactionWeb.timestamp);

        Transaction transaction;

        DigitalSignatureManager::hex_to_signature(transactionWeb.sender_signature, transaction.sender_signature);
        transaction.sender_signature_len = transactionWeb.sender_signature_len;
        const char *temp = transactionWeb.hash.c_str();
        memcpy(transaction.hash, temp, 32);
        DigitalSignatureManager::hex_to_public_key(transactionWeb.sender_public_key, transaction.sender_public_key);
        DigitalSignatureManager::hex_to_public_key(transactionWeb.receiver_public_key, transaction.receiver_public_key);
        transaction.amount = transactionWeb.amount;
        transaction.gas = transactionWeb.gas;
        transaction.timestamp = transactionWeb.timestamp;

        if (Validator::is_transaction_valid(transaction))
        {
            BlockchainStorage::add_transaction(transaction);
            ApplicationManager::broadcast_new_transaction(transaction);
            request->reply(status_codes::OK);
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