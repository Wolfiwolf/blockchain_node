#ifndef TX_IN_WITH_AMOUNT_H
#define TX_IN_WITH_AMOUNT_H

#include <string>
#include "transaction_in.hpp"

namespace BlockchainNode {

    struct TxInWithAmount {
        TxIn tx_in;
        unsigned long int amount;
    };

}

#endif