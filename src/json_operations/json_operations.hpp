#ifndef JSON_OPERATIONS_H
#define JSON_OPERATIONS_H

#include <string>
#include "../models/models.h"

namespace BlockchainNode
{
    class JsonOperations {
    public:
        static int get_int_value(const std::string& json_str, const std::string& key);
        static unsigned long int get_long_unsigned_int_value(const std::string &json_str, const std::string &key);
        static std::string get_str_value(const std::string& json_str, const std::string& key);

        static Transaction get_transaction(const std::string &json_str);
        static std::vector<TxInWithAmount> get_array_of_tx_ins_with_amount(const std::string &json_str);

        static std::string transaction_to_json(const Transaction& transaction);
        static std::string tx_in_to_json(const TxIn& tx_in);
        static std::string tx_out_to_json(const TxOut& tx_out);
    };

}

#endif