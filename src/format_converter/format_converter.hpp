#ifndef FORMAT_CONVERTER_H
#define FORMAT_CONVERTER_H

#include <string>

#include "../models/models.h"

namespace BlockchainNode
{

    class FormatConverter
    {
    public:
        static std::string bin_to_hex(uint8_t *bin, int bin_size);
        static void hex_to_bin(const std::string &hex, uint8_t *output);
    };

}

#endif