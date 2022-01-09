#include "hasher.hpp"

#include "../digital_signature_manager/digital_signature_manager.hpp"
#include "../debug/logger/logger.hpp"

namespace BlockchainNode
{

    Hasher::Hasher()
    {
    }

    std::string Hasher::hash_transaction(const Transaction &transaction)
    {
        std::string hash = Vendor::sha256(std::string((char *)(transaction.sender_public_key)));
        hash = Vendor::sha256(std::to_string(transaction.timestamp) + hash);
        hash = Vendor::sha256(std::string((char *)(transaction.receiver_public_key)) + hash);
        hash = Vendor::sha256(std::to_string(transaction.amount) + hash);
        return Vendor::sha256(std::to_string(transaction.gas) + hash);
    }

}
