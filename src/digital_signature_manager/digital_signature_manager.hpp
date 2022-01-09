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
        static bool verify_signature(uint8_t *public_key, uint8_t *signature, size_t signature_len, uint8_t *hash);

        static std::string private_key_to_hex(uint8_t *private_key);
        static std::string public_key_to_hex(uint8_t *public_key);
        static std::string signature_to_hex(uint8_t *signature, int signatire_len);

        static void hex_to_private_key(const std::string &private_key, uint8_t *output);
        static void hex_to_public_key(const std::string &public_key, uint8_t *output);
        static void hex_to_signature(const std::string &signature, uint8_t *output);

    private:
        static void sign_data(uint8_t *private_key, uint8_t *data_to_sign, int data_size, uint8_t *output, size_t *sig_size);
        static void generate_private_key(uint8_t *privateKey);
    };

}

#endif