#include "json_operations.hpp"

#include <regex>

#include "../debug/logger/logger.hpp"

namespace BlockchainNode
{

    JsonOperations::JsonOperations()
    {
    }

    int JsonOperations::get_int_value(const std::string &json_str, const std::string &key)
    {
        std::regex reg("\"" + key + "\":\\s*(-*\\d+)");
        std::smatch match;
        std::regex_search(json_str, match, reg);
        if (!match.empty())
        {
            return std::stoi(match[1]);
        }
        return -1;
    }

    std::string JsonOperations::get_str_value(const std::string &json_str, const std::string &key)
    {
        std::regex reg("\"" + key + "\":\\s*\"([^\"]+)\"");
        std::smatch match;
        std::regex_search(json_str, match, reg);
        if (!match.empty())
        {
            return match[1];
        }
        return "";
    }

}