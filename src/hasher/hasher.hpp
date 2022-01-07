#ifndef HASHER_H
#define HASHER_H

#include <string>

namespace BlockchainNode
{

    class Hasher
    {
    public:
        Hasher();

        static void hash_with_sha256(uint8_t *data, uint8_t *output);
    };

}

#endif