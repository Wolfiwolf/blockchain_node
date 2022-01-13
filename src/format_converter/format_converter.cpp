#include "format_converter.hpp"

#include <iostream>

namespace BlockchainNode
{
    static int char2int(char input)
    {
        if (input >= '0' && input <= '9')
            return input - '0';
        if (input >= 'A' && input <= 'F')
            return input - 'A' + 10;
        if (input >= 'a' && input <= 'f')
            return input - 'a' + 10;
        throw std::invalid_argument("Invalid input string");
    }

    static void hex2bin(const char *src, char *target)
    {
        while (*src && src[1])
        {
            *(target++) = char2int(*src) * 16 + char2int(src[1]);
            src += 2;
        }
    }

    std::string FormatConverter::bin_to_hex(uint8_t *bin, int bin_size)
    {
        std::string hex("");
        for (int i = 0; i < bin_size; ++i)
        {
            char l_pCharRes[3];
            sprintf(l_pCharRes, "%X", (int)bin[i]);
            std::string temp(l_pCharRes);

            hex += (temp.size() == 1 ? "0" + temp : temp);
        }
        return "0x" + hex;
    }

    void FormatConverter::hex_to_bin(const std::string &hex, uint8_t *output)
    {
        std::string hex_raw = hex.substr(2, hex.length() - 2);
        hex2bin(hex_raw.c_str(), (char *)output);
    }
}
