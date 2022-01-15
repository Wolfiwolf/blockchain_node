#include "message_serializer.hpp"

#include <string>
#include <cstring>
#include "../debug/logger/logger.hpp"

namespace BlockchainNode
{

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

        memcpy(output_bytes + offset, &block.timestamp, sizeof(int));
        offset += sizeof(int);

        memcpy(output_bytes + offset, &block.transactions, (64 + 4 + 4) * 3);
        offset += (64 + 4 + 4) * 3;
    }

    Block MessageSerializer::bytes_to_block(const uint8_t *bytes)
    {
        Block block;

        int offset = 0;

        memcpy(&block.id, bytes + offset, sizeof(int));
        offset += sizeof(int);

        memcpy(&block.nonce, bytes + offset, sizeof(int));
        offset += sizeof(int);

        memcpy(&block.hash, bytes + offset, 32);
        offset += 32;

        memcpy(&block.hash_of_previous_block, bytes + offset, 32);
        offset += 32;

        memcpy(&block.timestamp, bytes + offset, sizeof(int));
        offset += sizeof(int);

        memcpy(&block.transactions, bytes + offset, (64 + 4 + 4) * 3);
        offset += (64 + 4 + 4) * 3;

        return block;
    }

    int MessageSerializer::transaction_to_bytes(const Transaction &transaction, uint8_t *output_bytes)
    {
        int offset = 0;

        memcpy(output_bytes + offset, &transaction.timestamp, 8);
        offset += 8;

        int size = transaction.sender_signature.size();
        memcpy(output_bytes + offset, &size, 4);
        offset += 4;

        memcpy(output_bytes + offset, transaction.sender_signature.c_str(), size);
        offset += size;

        memcpy(output_bytes + offset, transaction.sender_public_key.c_str(), transaction.sender_public_key.size());
        offset += transaction.sender_public_key.size();

        memcpy(output_bytes + offset, transaction.hash.c_str(), transaction.hash.size());
        offset += transaction.hash.size();

        memcpy(output_bytes + offset, &transaction.gas, 4);
        offset += 4;

        int num_of_transaction_ins = transaction.tx_ins.size();
        memcpy(output_bytes + offset, &num_of_transaction_ins, 4);
        offset += 4;

        for (const TxIn &tx_in : transaction.tx_ins)
        {
            memcpy(output_bytes + offset, &tx_in.block_id, 4);
            offset += 4;

            memcpy(output_bytes + offset, &tx_in.tx_out_index, 4);
            offset += 4;

            memcpy(output_bytes + offset, tx_in.transaction_hash.c_str(), tx_in.transaction_hash.size());
            offset += tx_in.transaction_hash.size();
        }

        num_of_transaction_ins = transaction.tx_outs.size();
        memcpy(output_bytes + offset, &num_of_transaction_ins, 4);
        offset += 4;

        for (const TxOut &tx_out : transaction.tx_outs)
        {
            memcpy(output_bytes + offset, &tx_out.index, 4);
            offset += 4;

            memcpy(output_bytes + offset, &tx_out.amount, 8);
            offset += 8;

            memcpy(output_bytes + offset, tx_out.receiver_public_key.c_str(), 33 * 2 + 2);
            offset += 33 * 2 + 2;
        }

        return offset;
    }

    Transaction MessageSerializer::bytes_to_transaction(const uint8_t *bytes)
    {
        Transaction transaction;

        int offset = 0;

        memcpy(&transaction.timestamp, bytes + offset, 8);
        offset += 8;

        int size;
        memcpy(&size, bytes + offset, 4);
        offset += 4;

        char temp[512];
        memcpy(temp, bytes + offset, size);
        transaction.sender_signature = std::string(temp, size);
        offset += size;

        memcpy(temp, bytes + offset, 33 * 2 + 2);
        transaction.sender_public_key = std::string(temp, 33 * 2 + 2);
        offset += 33 * 2 + 2;

        memcpy(temp, bytes + offset, 32 * 2 + 2);
        transaction.hash = std::string(temp, 32 * 2 + 2);
        offset += 32 * 2 + 2;

        memcpy(&transaction.gas, bytes + offset, 4);
        offset += 4;

        int num_of_transaction_ins;
        memcpy(&num_of_transaction_ins, bytes + offset, 4);
        offset += 4;

        for (int i = 0; i < num_of_transaction_ins; ++i)
        {
            TxIn tx_in;
            memcpy(&tx_in.block_id, bytes + offset, 4);
            offset += 4;

            memcpy(&tx_in.tx_out_index, bytes + offset, 4);
            offset += 4;

            memcpy(temp, bytes + offset, 32 * 2 + 2);
            tx_in.transaction_hash = std::string(temp, 32 * 2 + 2);
            offset += 32 * 2 + 2;

            transaction.tx_ins.push_back(tx_in);
        }

        int num_of_transaction_outs;
        memcpy(&num_of_transaction_outs, bytes + offset, 4);
        offset += 4;
        for (int i = 0; i < num_of_transaction_outs; ++i)
        {
            TxOut tx_out;

            memcpy(&tx_out.index, bytes + offset, 4);
            offset += 4;

            memcpy(&tx_out.amount, bytes + offset, 8);
            offset += 8;

            memcpy(temp, bytes + offset, 33 * 2 + 2);
            tx_out.receiver_public_key = std::string(temp, 33 * 2 + 2);
            offset += 33 * 2 + 2;

            transaction.tx_outs.push_back(tx_out);
        }

        return transaction;
    }

}