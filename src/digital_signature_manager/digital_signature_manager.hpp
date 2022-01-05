#ifndef DIGITAL_SIGNATURE_MANAGER_H
#define DIGITAL_SIGNATURE_MANAGER_H

#include <string>

#include "../models/models.h"

namespace BlockchainNode
{

    class DigitalSignatureManager
    {
    public:
        static Wallet generate_wallet();

    private:
        static void generate_private_key(uint8_t *privateKey);
    };

}

#endif