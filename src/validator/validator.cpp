#include "validator.hpp"

#include <string>

#include "../digital_signature_manager/digital_signature_manager.hpp"
#include "../debug/logger/logger.hpp"
#include "../hasher/hasher.hpp"
#include "../format_converter/format_converter.hpp"

namespace BlockchainNode
{

    bool Validator::is_block_valid(const Block &block)
    {

        return true;
    }

    bool Validator::is_transaction_valid(const Transaction &transaction)
    {
        std::string hash = Hasher::hash_transaction(transaction);

        if (hash != transaction.hash)
            return false;


        int res = DigitalSignatureManager::verify_transaction_signature(transaction);

        return true;
    }

}