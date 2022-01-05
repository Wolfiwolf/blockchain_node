#ifndef MESSAGE_SERIALIZER_H
#define MESSAGE_SERIALIZER_H

#include "../models/models.h"

namespace BlockchainNode
{

    class MessageSerializer
    {
    public:
        static void block_to_bytes(const Block &block, uint8_t *output_bytes);
        static Block bytes_to_block(const uint8_t *bytes);
        
        static void transaction_to_bytes(const Transaction &transaction, uint8_t *output_bytes);
        static Transaction bytes_to_transaction(const uint8_t *bytes);
    };

}

#endif