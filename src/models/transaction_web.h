#ifndef TRANSACTION_WEB_H
#define TRANSACTION_WEB_H

#include <string>

namespace BlockchainNode {

    struct TransactionWeb {
        unsigned long int timestamp;
        std::string sender_signature;
        int sender_signature_len;
        std::string hash;
        std::string sender_public_key;
        std::string receiver_public_key;
        int amount;
        int gas;
    };

}

#endif