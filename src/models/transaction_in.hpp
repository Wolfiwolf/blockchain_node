#ifndef TRANSACTION_IN_H
#define TRANSACTION_IN_H

#include <string>

namespace BlockchainNode {

    struct TxIn {
        int block_id;
        int tx_out_index;
        std::string transaction_hash;
    };

}

#endif