#include "communicator.hpp"

#include <thread>

#include "../debug/logger/logger.hpp"

namespace BlockchainNode
{
    Communicator::Communicator()
    {
    }

    void Communicator::start_listening(int litening_port)
    {
        int listening = socket(AF_INET, SOCK_STREAM, 0);
        if (listening == -1)
        {
            LOG_WNL("Can't create a socket! Quitting");
            return;
        }

        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(litening_port);
        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

        bind(listening, (sockaddr *)&hint, sizeof(hint));

        listen(listening, SOMAXCONN);

        while (true)
        {
            sockaddr_in client;
            socklen_t clientSize = sizeof(client);

            int client_socket = accept(listening, (sockaddr *)&client, &clientSize);

            LOG_WNL("Client connected on port");

            std::thread thrd(deal_with_client, this, client_socket);
        }

        close(listening);
    }

    bool Communicator::send_to(const char *ip_address, int port, const char *data, int data_size)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            return 1;
        }

        std::string ipAddress = ip_address;

        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

        int connectRes = connect(sock, (sockaddr *)&hint, sizeof(hint));
        if (connectRes == -1)
        {
            return 1;
        }

        char buf[4096];

        int sendRes = send(sock, data, data_size + 1, 0);
        if (sendRes == -1)
        {
            std::cout << "Could not send to server! Whoops!\r\n";
            return false;
        }

        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            std::cout << "There was an error getting response from server\r\n";
            return false;
        }
        else
        {
            std::cout << "SERVER> " << std::string(buf, bytesReceived) << "\r\n";
        }

        close(sock);
        
        return true;
    }

    void Communicator::set_on_message_received_callback(void (*on_message_received_callback)(uint8_t *data, int data_size))
    {
        _on_message_received_callback = on_message_received_callback;
    }

    void Communicator::deal_with_client(Communicator *communicator, int client_socket)
    {
        uint8_t buf[4096];

        memset(buf, 0, 4096);

        int bytesReceived = recv(client_socket, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            LOG_WNL("Error in recv(). Quitting");
            return;
        }

        if (bytesReceived == 0)
        {
            LOG_WNL("Client disconnected");
            return;
        }

        LOG_WNL("Data received: " << std::string((char *)buf, 0, bytesReceived));

        communicator->_on_message_received_callback(buf, bytesReceived);

        close(client_socket);
    }

}