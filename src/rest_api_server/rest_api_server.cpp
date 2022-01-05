#include "rest_api_server.hpp"

#include <cpprest/http_listener.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "../debug/logger/logger.hpp"

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
    }

    void RestApiServer::handle_get_request(http_request request)
    {
        std::string path = request.request_uri().path();

        LOG_WNL("#################");
        LOG_WNL("REST API: " << "New request received " << path);

        std::vector<std::string> path_parts = get_path_parts(path);


        if (path_parts[0] == "balance-of") 
        {
            if (path_parts.size() == 2)
            {
                std::string response = on_balance_of_request(path_parts[1]);
                LOG_WNL("REST API: Sending response: " << response);
                request.reply(status_codes::OK, response);
                return;
            }
        }
        
        request.reply(status_codes::NotFound);
    }

    std::string RestApiServer::on_balance_of_request(const std::string &address)
    {
        return "1000000";
    }

    std::vector<std::string> RestApiServer::get_path_parts(const std::string &path)
    {
        std::vector<std::string> result;

        std::string current("");
        for(int i = 1; i < path.size(); ++i)
        {
            if (path[i] == '/') 
            {
                result.push_back(current);
                current = "";
            }
            else current += path[i];
        }

        if (current != "") result.push_back(current);
    
        return result;
    }

}