#ifndef HASHER_H
#define HASHER_H

#include <string>
#include "../vendor/sha256/sha256.hpp"
#include "../models/models.h"

namespace BlockchainNode
{

    class Hasher
    {
    public:
        Hasher();

        static std::string hash_transaction(const Transaction &transaction);
    };

}

#endif