#ifndef MESSAGE_SERIALIZER_H
#define MESSAGE_SERIALIZER_H

#include <vector>
#include <cpprest/http_listener.h>

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

        static std::string on_balance_of_request(const std::string &address);

        static std::vector<std::string> get_path_parts(const std::string &path);
    };
}

#endif