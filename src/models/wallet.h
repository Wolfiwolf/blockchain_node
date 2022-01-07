#ifndef WALLET_H
#define WALLET_H

#include <string>

namespace BlockchainNode {

    struct Wallet {
        uint8_t privateKey[32];
        uint8_t publicKey[33];
    };

}

#endif