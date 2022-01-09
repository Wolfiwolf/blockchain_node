#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

namespace BlockchainNode {

    struct Transaction {
        unsigned long int timestamp;
        uint8_t sender_signature[72];
        size_t sender_signature_len;
        uint8_t hash[32];
        uint8_t sender_public_key[33];
        uint8_t receiver_public_key[33];
        int amount;
        int gas;
    };

}

#endif