#ifndef DIGITAL_SIGNATURE_MANAGER_H
#define DIGITAL_SIGNATURE_MANAGER_H

#include <string>
#include <secp256k1.h>

#include "../models/models.h"

namespace BlockchainNode
{

    class DigitalSignatureManager
    {
    public:
        DigitalSignatureManager();
        static Wallet generate_wallet();

        static void sign_transaction(uint8_t *private_key, Transaction *transaction);
        static bool verify_transaction_signature(const Transaction &transaction);

    private:
        static void sign_data(uint8_t *private_key, uint8_t *data_to_sign, int data_size, uint8_t *output, size_t *sig_size);
        static void generate_private_key(uint8_t *privateKey);
    };

}

#endif