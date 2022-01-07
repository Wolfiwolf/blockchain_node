#include "message_serializer.hpp"

#include <string>
#include <cstring>

namespace BlockchainNode {

    void MessageSerializer::block_to_bytes(const Block &block, uint8_t *output_bytes) 
    {
        int offset = 0;

        memcpy(output_bytes + offset, &block.id, sizeof(int));
        offset += sizeof(int);

        memcpy(output_bytes + offset, &block.nonce, sizeof(int));
        offset += sizeof(int);

        memcpy(output_bytes + offset, &block.hash, 32);
        offset += 32;

        memcpy(output_bytes + offset, &block.hash_of_previous_block, 32);
        offset += 32;

        memcpy(output_bytes + offset, &block.reward_to_miner, sizeof(int));
        offset += sizeof(int);

        memcpy(output_bytes + offset, &block.timestamp, sizeof(int));
        offset += sizeof(int);

        memcpy(output_bytes + offset, &block.transactions, (64 + 4 + 4) * 3);
        offset += (64 + 4 + 4) * 3;

        memcpy(output_bytes + offset, &block.minerPublicKey, 33);
    }

    Block MessageSerializer::bytes_to_block(const uint8_t *bytes) 
    {
        Block block;

        int offset = 0;

        memcpy(&block.id, bytes + offset, sizeof(int));
        offset += sizeof(int);

        memcpy(&block.nonce, bytes + offset,  sizeof(int));
        offset += sizeof(int);

        memcpy(&block.hash, bytes + offset, 32);
        offset += 32;

        memcpy(&block.hash_of_previous_block, bytes + offset, 32);
        offset += 32;

        memcpy(&block.reward_to_miner, bytes + offset, sizeof(int));
        offset += sizeof(int);

        memcpy(&block.timestamp, bytes + offset, sizeof(int));
        offset += sizeof(int);

        memcpy(&block.transactions, bytes + offset, (64 + 4 + 4) * 3);
        offset += (64 + 4 + 4) * 3;

        memcpy(&block.minerPublicKey, bytes + offset, 33);

        return block;
    }

    void MessageSerializer::transaction_to_bytes(const Transaction &transaction, uint8_t *output_bytes)
    {
        int offset = 0;

        memcpy(output_bytes + offset, &transaction.sender_public_key, 33);
        offset += 33;

        memcpy(output_bytes + offset, &transaction.receiver_public_key, 33);
        offset += 33;

        memcpy(output_bytes + offset, &transaction.amount, sizeof(int));
        offset += sizeof(int);

        memcpy(output_bytes + offset, &transaction.gas, sizeof(int));
    }

    Transaction MessageSerializer::bytes_to_transaction(const uint8_t *bytes)
    {
        Transaction transaction;
        int offset = 0;

        memcpy(&transaction.sender_public_key, bytes + offset, 33);
        offset += 33;

        memcpy(&transaction.receiver_public_key, bytes + offset, 33);
        offset += 33;

        memcpy(&transaction.amount, bytes + offset, sizeof(int));
        offset += sizeof(int);

        memcpy(&transaction.gas, bytes + offset, sizeof(int));

        return transaction;
    }


}