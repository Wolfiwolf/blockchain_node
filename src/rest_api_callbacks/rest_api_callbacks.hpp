#ifndef REST_API_CALLBACKS_H
#define REST_API_CALLBACKS_H

#include <cpprest/http_listener.h>

#include "../models/models.h"
#include "../application_manager/application_manager.hpp"

using namespace web::http::experimental::listener;
using namespace web::http;
using namespace web;

namespace BlockchainNode
{
    namespace RestApiCallbacks
    {

        void on_unspent_transactions_request(http_request *request, const std::string &public_key);
        void on_new_node_request(http_request *request, const BlockchainNodeContactInfo &nodeContactInfo);
        void on_new_transaction_request(http_request *request, const Transaction &transaction);

    }
}

#endif