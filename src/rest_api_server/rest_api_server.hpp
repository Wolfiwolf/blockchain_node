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

        static RestApiServer *get_instance();

        void start_server(int listening_port);

        void set_on_unspent_transactions_request_callback(void (*on_unspent_transactions_request_callback)(http_request *request, const std::string &public_key));
        void set_on_new_node_request_callback(void (*on_new_node_request_callback)(http_request *request, const BlockchainNodeContactInfo &nodeContactInfo));
        void set_on_new_transaction_request_callback(void (*on_new_transaction_request_callback)(http_request *request, const Transaction &transaction));

    private:
        static RestApiServer *_instance;
        void (*_on_unspent_transactions_request_callback)(http_request *request, const std::string &public_key);
        void (*_on_new_node_request_callback)(http_request *request, const BlockchainNodeContactInfo &nodeContactInfo);
        void (*_on_new_transaction_request_callback)(http_request *request, const Transaction &transaction);

        http_listener *_listener;

        static void handle_post_request(http_request request);
        static void handle_get_request(http_request request);

        static std::vector<std::string> get_path_parts(const std::string &path);
    };
}

#endif