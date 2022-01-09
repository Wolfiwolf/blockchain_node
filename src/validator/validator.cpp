#include "validator.hpp"

#include <string>

#include "../digital_signature_manager/digital_signature_manager.hpp"
#include "../debug/logger/logger.hpp"
#include "../hasher/hasher.hpp"

namespace BlockchainNode
{

    bool Validator::is_block_valid(const Block &block)
    {

        return true;
    }

    bool Validator::is_transaction_valid(const Transaction &transaction)
    {
        std::string hash = Hasher::hash_transaction(transaction);

        if (hash.substr(0, 32) != std::string((char *)transaction.hash, 32))
            return false;

        int res = DigitalSignatureManager::verify_signature(
            (uint8_t *)transaction.sender_public_key,
            (uint8_t *)transaction.sender_signature,
            transaction.sender_signature_len,
            (uint8_t *)transaction.hash);

        LOG_WNL((res ? "Is valid" : "Is not valid"));
        return true;
    }

}