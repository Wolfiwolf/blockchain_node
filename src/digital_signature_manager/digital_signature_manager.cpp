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
#include "../hasher/hasher.hpp"

namespace BlockchainNode
{
    int char2int(char input)
    {
        if (input >= '0' && input <= '9')
            return input - '0';
        if (input >= 'A' && input <= 'F')
            return input - 'A' + 10;
        if (input >= 'a' && input <= 'f')
            return input - 'a' + 10;
        throw std::invalid_argument("Invalid input string");
    }
    void hex2bin(const char *src, char *target)
    {
        while (*src && src[1])
        {
            *(target++) = char2int(*src) * 16 + char2int(src[1]);
            src += 2;
        }
    }

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
        std::string hash = Hasher::hash_transaction(*transaction);
        

        memcpy(transaction->hash, hash.c_str(), 32);

        transaction->sender_signature_len = 72;
        sign_data(private_key, transaction->hash, 32, transaction->sender_signature, &transaction->sender_signature_len);
    }

    bool DigitalSignatureManager::verify_signature(uint8_t *public_key, uint8_t *signature, size_t signature_len, uint8_t *hash)
    {
        secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

        secp256k1_pubkey pubkey;
        secp256k1_ecdsa_signature sig;

        if (secp256k1_ec_pubkey_parse(ctx, &pubkey, public_key, 33) == 0) {
            LOG_WNL("Error parsing public key!");
        }

        if (secp256k1_ecdsa_signature_parse_der(ctx, &sig, signature, signature_len) == 0) {
            LOG_WNL("Error parsing signature!");
        }

        int res = secp256k1_ecdsa_verify(ctx, &sig, hash, &pubkey);


        return res == 1;
    }

    std::string DigitalSignatureManager::private_key_to_hex(uint8_t *private_key)
    {
        std::string private_key_hex("");
        for (int i = 0; i < 32; ++i)
        {
            char l_pCharRes[3];
            sprintf(l_pCharRes, "%X", (int)private_key[i]);
            std::string temp(l_pCharRes);

            private_key_hex += (temp.size() == 1 ? "0" + temp : temp);
        }
        return "0x" + private_key_hex;
    }

    std::string DigitalSignatureManager::public_key_to_hex(uint8_t *public_key)
    {
        std::string public_key_hex("");
        for (int i = 0; i < 33; ++i)
        {
            char l_pCharRes[3];
            sprintf(l_pCharRes, "%X", (int)public_key[i]);
            std::string temp(l_pCharRes);

            public_key_hex += (temp.size() == 1 ? "0" + temp : temp);
        }
        return "0x" + public_key_hex;
    }

    void DigitalSignatureManager::hex_to_private_key(const std::string &private_key, uint8_t *output)
    {
        std::string private_key_raw = private_key.substr(2, private_key.length() - 2); 
        hex2bin(private_key_raw.c_str(), (char*)output);
    }
    void DigitalSignatureManager::hex_to_public_key(const std::string &public_key, uint8_t *output)
    {
        std::string public_key_raw = public_key.substr(2, public_key.length() - 2); 
        hex2bin(public_key_raw.c_str(), (char*)output);
    }

    void DigitalSignatureManager::hex_to_signature(const std::string &signature, uint8_t *output)
    {
        std::string signature_raw = signature.substr(2, signature.length() - 2); 
        hex2bin(signature_raw.c_str(), (char*)output);
    }

    std::string DigitalSignatureManager::signature_to_hex(uint8_t *signature, int signatire_len)
    {
        std::string signature_hex("");
        for (int i = 0; i < signatire_len; ++i)
        {
            char l_pCharRes[3];
            sprintf(l_pCharRes, "%X", (int)signature[i]);
            std::string temp(l_pCharRes);

            signature_hex += (temp.size() == 1 ? "0" + temp : temp);
        }
        return "0x" + signature_hex;
    }

    void DigitalSignatureManager::sign_data(uint8_t *private_key, uint8_t *data_to_sign, int data_size, uint8_t *output, size_t *sig_size)
    {
        secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
        size_t signature_length = 72;
        secp256k1_ecdsa_signature sig;

        secp256k1_ecdsa_sign(ctx, &sig, data_to_sign, private_key, NULL, NULL);
        secp256k1_ecdsa_signature_serialize_der(ctx, output, sig_size, &sig);

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
