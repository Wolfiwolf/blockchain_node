#include "rest_api_server.hpp"

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

    RestApiServer *RestApiServer::_instance;

    RestApiServer::RestApiServer()
    {
        _instance = this;
    }

    RestApiServer::~RestApiServer()
    {
        delete _listener;
    }

    RestApiServer *RestApiServer::get_instance()
    {
        return _instance;
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

            RestApiServer::get_instance()->_on_new_transaction_request_callback(&request, transaction);
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

            RestApiServer::get_instance()->_on_new_node_request_callback(&request, nodeInfo);
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
                RestApiServer::get_instance()->_on_unspent_transactions_request_callback(&request, path_parts[1]);
                return;
            }
        }

        request.reply(status_codes::NotFound);
    }

    void RestApiServer::set_on_unspent_transactions_request_callback(void (*on_unspent_transactions_request_callback)(http_request *request, const std::string &public_key))
    {
        _on_unspent_transactions_request_callback = on_unspent_transactions_request_callback;
    }

    void RestApiServer::set_on_new_node_request_callback(void (*on_new_node_request_callback)(http_request *request, const BlockchainNodeContactInfo &nodeContactInfo))
    {
        _on_new_node_request_callback = on_new_node_request_callback;
    }

    void RestApiServer::set_on_new_transaction_request_callback(void (*on_new_transaction_request_callback)(http_request *request, const Transaction &transaction))
    {
        _on_new_transaction_request_callback = on_new_transaction_request_callback;
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