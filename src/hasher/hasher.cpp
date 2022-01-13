#include "hasher.hpp"

#include <chrono>
#include "../digital_signature_manager/digital_signature_manager.hpp"
#include "../debug/logger/logger.hpp"

namespace BlockchainNode
{

    Hasher::Hasher()
    {
    }

    std::string Hasher::hash_block(Block &block)
    {
        const auto p1 = std::chrono::system_clock::now();
        block.timestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();

        std::string hash = Vendor::sha256(std::to_string(block.id));
        hash = Vendor::sha256(std::to_string(block.timestamp) + hash);
        hash = Vendor::sha256(std::to_string(block.nonce) + hash);
        hash = Vendor::sha256(block.hash_of_previous_block + hash);
        for (const Transaction &transaction : block.transactions)
        {
            hash = Hasher::hash_transaction(transaction) + hash;
        }
        
        block.hash = hash;
        return hash;
    }

    std::string Hasher::hash_transaction(const Transaction &transaction)
    {
        std::string hash = Vendor::sha256(std::to_string(transaction.timestamp));
        hash = Vendor::sha256(transaction.sender_public_key + hash);
        hash = Vendor::sha256(std::to_string(transaction.gas) + hash);
        
        for(const TxIn &tx_in : transaction.tx_ins)
        {
            hash = Vendor::sha256(std::to_string(tx_in.block_id) + hash);
            hash = Vendor::sha256(tx_in.transaction_hash + hash);
            hash = Vendor::sha256(std::to_string(tx_in.tx_out_index) + hash);
        }
        
        for(const TxOut &tx_out : transaction.tx_outs)
        {
            hash = Vendor::sha256(std::to_string(tx_out.index) + hash);
            hash = Vendor::sha256(std::to_string(tx_out.amount) + hash);
            hash = Vendor::sha256(tx_out.receiver_public_key + hash);
        }

        return "0x" + hash;
    }

}
