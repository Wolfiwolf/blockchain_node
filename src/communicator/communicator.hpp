#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include "../models/models.h"

namespace BlockchainNode {
    class Communicator {
    public:
        Communicator();

        void start_listening(int litening_port);
        bool send_to(const char *ip_address, int port, const char *data, int data_size);
        void set_on_message_received_callback(void (*on_message_received_callback)(uint8_t *data, int data_size));
    private:
        void (*_on_message_received_callback)(uint8_t *data, int data_size);

        static void deal_with_client(Communicator *communicator, int client_socket);
    };
}

#endif