#ifndef TRANSACTION_OUT_H
#define TRANSACTION_OUT_H

#include <string>

namespace BlockchainNode
{

    struct TxOut
    {
        int index;
        unsigned long int amount;
        std::string receiver_public_key;
    };

}

#endif