#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include "transaction_in.hpp"
#include "transaction_out.hpp"

namespace BlockchainNode {

    struct Transaction {
        unsigned long int timestamp;
        std::string sender_signature;
        std::string sender_public_key;
        std::string hash;
        int gas;
        std::vector<TxIn> tx_ins;
        std::vector<TxOut> tx_outs;
    };

}

#endif