#include <string>

#include <stdexcept>
#include <random>
#include <iostream>
#include <secp256k1.h>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

#include "../debug/logger/logger.hpp"
#include "digital_signature_manager.hpp"

namespace BlockchainNode
{

    DigitalSignatureManager::DigitalSignatureManager()
    {
    }

    Wallet DigitalSignatureManager::generate_wallet()
    {
        LOG_WNL("Generating wallet...");
        LOG_NL();

        uint8_t private_key[32];
        generate_private_key(private_key);

        secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);

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
        LOG_WNL("Public key created:");
        for (int i = 0; i < 32; ++i)
        {
            char l_pCharRes[3];
            sprintf(l_pCharRes, "%X", (int)pubkey_serialized[i]);
            std::string temp(l_pCharRes);

            LOG((temp.size() == 1 ? "0" + temp : temp));
        }
        LOG_NL();
        #endif



        Wallet wal;
        memcpy(wal.publicKey, pubkey_serialized, 32);
        memcpy(wal.publicKey, private_key, 32);

        return wal;
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
        LOG_WNL("Private key created:");
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
