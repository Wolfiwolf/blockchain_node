#ifndef REST_API_SERVER_H
#define REST_API_SERVER_H

#include <vector>
#include <cpprest/http_listener.h>

#include "../models/models.h"

using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;

namespace BlockchainNode
{

    class RestApiServer
    {
    public:
        RestApiServer();
        ~RestApiServer();

        void start_server(int listening_port);

    private:
        http_listener *_listener;

        static void handle_post_request(http_request request);
        static void handle_get_request(http_request request);

        static void on_unspent_transactions_request(http_request *request, const std::string &public_key);

        static void on_new_node_request(http_request *request, const BlockchainNodeContactInfo &nodeContactInfo);
        static void on_new_transaction_request(http_request *request, const Transaction &transaction);
        
        static std::vector<std::string> get_path_parts(const std::string &path);
    };
}

#endif