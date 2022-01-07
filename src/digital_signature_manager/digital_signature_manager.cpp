#include <string>
#include <stdexcept>
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

#include "../debug/logger/logger.hpp"
#include "digital_signature_manager.hpp"
#include "../vendor/sha256/sha256.hpp"

namespace BlockchainNode
{

    DigitalSignatureManager::DigitalSignatureManager()
    {
    }

    Wallet DigitalSignatureManager::generate_wallet()
    {
        uint8_t private_key[32];
        generate_private_key(private_key);

        secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

        if (secp256k1_ec_seckey_verify(ctx, private_key) == 0)
        {
            fprintf(stderr, "Invalid secret key\n");
            exit(1);
        }

        secp256k1_pubkey pubkey;
        int ret = secp256k1_ec_pubkey_create(ctx, &pubkey, private_key);
        if (ret == 0)
        {
            fprintf(stderr, "Invalid secret key 2\n");
            exit(1);
        }
        uint8_t pubkey_serialized[33];
        size_t pubkeylen = 33;

        secp256k1_ec_pubkey_serialize(ctx, pubkey_serialized, &pubkeylen, &pubkey, SECP256K1_EC_COMPRESSED);

        secp256k1_pubkey pubkey2;
        if (secp256k1_ec_pubkey_parse(ctx, &pubkey2, pubkey_serialized, 33) == 0)
        {
            printf("error re-parsing public key!\n");
            exit(1);
        }

        secp256k1_context_destroy(ctx);

        LOG_NL();

#if DEBUG_MODE == 1
        LOG_WNL("PUBLIC KEY:");
        for (int i = 0; i < 33; ++i)
        {
            char l_pCharRes[3];
            sprintf(l_pCharRes, "%X", (int)pubkey_serialized[i]);
            std::string temp(l_pCharRes);

            LOG((temp.size() == 1 ? "0" + temp : temp));
        }
        LOG_NL();
#endif

        Wallet wal;
        memcpy(wal.publicKey, pubkey_serialized, 33);
        memcpy(wal.privateKey, private_key, 32);

        return wal;
    }

    void DigitalSignatureManager::sign_transaction(uint8_t *private_key, Transaction *transaction)
    {
        std::string hash = Vendor::sha256(std::string((char *)(transaction->sender_public_key)));
        hash = Vendor::sha256(std::string((char *)(transaction->receiver_public_key)) + hash);
        hash = Vendor::sha256(std::to_string(transaction->amount) + hash);
        hash = Vendor::sha256(std::to_string(transaction->gas) + hash);

        memcpy(transaction->hash, hash.c_str(), 33);

        transaction->sender_signature_len = 72;
        sign_data(private_key, transaction->hash, 32, transaction->sender_signature, &transaction->sender_signature_len);
    }

    bool DigitalSignatureManager::verify_signature(uint8_t *public_key, uint8_t *signature, int signature_len, uint8_t *hash)
    {
        secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

        secp256k1_pubkey pubkey;
        secp256k1_ecdsa_signature sig;

        secp256k1_ec_pubkey_parse(ctx, &pubkey, public_key, 33);
        secp256k1_ecdsa_signature_parse_der(ctx, &sig, signature, (size_t)signature_len);

        int res = secp256k1_ecdsa_verify(ctx, &sig, hash, &pubkey);

        return res == 1;
    }

    void DigitalSignatureManager::sign_data(uint8_t *private_key, uint8_t *data_to_sign, int data_size, uint8_t *output, size_t *sig_size)
    {
        secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
        size_t signature_length = 72;
        secp256k1_ecdsa_signature sig;

        secp256k1_ecdsa_sign(ctx, &sig, data_to_sign, private_key, NULL, NULL);
        secp256k1_ecdsa_signature_serialize_der(ctx, output, (size_t *)sig_size, &sig);

        secp256k1_context_destroy(ctx);
    }

    void DigitalSignatureManager::generate_private_key(uint8_t *privateKey)
    {
        const char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 256); // distribution in range [1, 6]

        for (int i = 0; i < 32; ++i)
        {
            privateKey[i] = dist(rng);
        }

#if DEBUG_MODE == 1
        LOG_WNL("PRIVATE KEY:");
        for (int i = 0; i < 32; ++i)
        {
            char l_pCharRes[3];
            sprintf(l_pCharRes, "%X", (int)privateKey[i]);
            std::string temp(l_pCharRes);

            LOG((temp.size() == 1 ? "0" + temp : temp));
        }
        LOG_NL();
#endif
    }
}
