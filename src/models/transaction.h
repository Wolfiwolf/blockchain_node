#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

namespace BlockchainNode {

    struct Transaction {
        uint8_t senderPublicKey[32];
        uint8_t receiverPublicKey[32];
        int amount;
        int gass;
    };

}

#endif