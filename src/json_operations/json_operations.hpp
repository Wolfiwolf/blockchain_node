#ifndef JSON_OPERATIONS_H
#define JSON_OPERATIONS_H

#include <string>

namespace BlockchainNode
{
    class JsonOperations {
    public:
        JsonOperations();
        static int get_int_value(const std::string& json_str, const std::string& key);
        static std::string get_str_value(const std::string& json_str, const std::string& key);
    };

}

#endif