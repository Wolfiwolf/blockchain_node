#include "message_serializer.hpp"

#include <string>
#include <cstring>
#include "../debug/logger/logger.hpp"

namespace BlockchainNode
{

    int MessageSerializer::block_to_bytes(const Block &block, uint8_t *output_bytes)
    {
        int offset = 0;

        memcpy(output_bytes + offset, &block.id, 4);
        offset += 4;

        memcpy(output_bytes + offset, &block.timestamp, 8);
        offset += 8;

        memcpy(output_bytes + offset, &block.nonce, 4);
        offset += 4;

        memcpy(output_bytes + offset, &block.difficulty, 4);
        offset += 4;

        memcpy(output_bytes + offset, block.hash.c_str(), 32 * 2 + 2);
        offset += 32 * 2 + 2;

        memcpy(output_bytes + offset, block.hash_of_previous_block.c_str(), 32 * 2 + 2);
        offset += 32 * 2 + 2;

        memcpy(output_bytes + offset, block.miner_public_key.c_str(), 33 * 2 + 2);
        offset += 33 * 2 + 2;

        int num_of_transactions = block.transactions.size();
        memcpy(output_bytes + offset, &num_of_transactions, 4);
        offset += 4;

        for (const Transaction &transaction : block.transactions)
        {
            int bytes = transaction_to_bytes(transaction, output_bytes + offset);
            offset += bytes;
        }

        return offset;
    }

    Block MessageSerializer::bytes_to_block(const uint8_t *bytes)
    {
        Block block;

        int offset = 0;

        memcpy(&block.id, bytes + offset, 4);
        offset += 4;

        memcpy(&block.timestamp, bytes + offset, 8);
        offset += 8;

        memcpy(&block.nonce, bytes + offset, 4);
        offset += 4;

        memcpy(&block.difficulty, bytes + offset, 4);
        offset += 4;

        char temp[1028];
        memcpy(temp, bytes + offset, 32 * 2 + 2);
        block.hash = std::string(temp, 32 * 2 + 2);
        offset += 32 * 2 + 2;

        memcpy(temp, bytes + offset, 32 * 2 + 2);
        block.hash_of_previous_block = std::string(temp, 32 * 2 + 2);
        offset += 32 * 2 + 2;

        memcpy(temp, bytes + offset, 33 * 2 + 2);
        block.miner_public_key = std::string(temp, 33 * 2 + 2);
        offset += 33 * 2 + 2;

        int num_of_transactions;
        memcpy(&num_of_transactions, bytes + offset, 4);
        offset += 4;

        for (int i = 0; i < num_of_transactions; ++i)
        {
            int transaction_bytes_len;
            Transaction transaction = bytes_to_transaction(bytes + offset, &transaction_bytes_len);
            offset += transaction_bytes_len;

            block.transactions.push_back(transaction);
        }

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

    Transaction MessageSerializer::bytes_to_transaction(const uint8_t *bytes, int *num_of_bytes)
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

        if (num_of_bytes != nullptr)
            *num_of_bytes = offset;

        return transaction;
    }

}