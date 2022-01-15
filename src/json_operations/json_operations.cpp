#include "json_operations.hpp"

#include <regex>

#include "../debug/logger/logger.hpp"

namespace BlockchainNode
{

    int JsonOperations::get_int_value(const std::string &json_str, const std::string &key)
    {
        std::regex reg("\"" + key + "\":\\s(-*\\d+)");
        std::smatch match;
        std::regex_search(json_str, match, reg);
        if (!match.empty())
        {
            return std::stoi(match[1]);
        }
        return -1;
    }

    unsigned long int JsonOperations::get_unsigned_long_int_value(const std::string &json_str, const std::string &key)
    {
        std::regex reg("\"" + key + "\":\\s(-*\\d+)");
        std::smatch match;
        std::regex_search(json_str, match, reg);
        if (!match.empty())
        {
            return std::stoul(match[1]);
        }
        return 0;
    }

    std::string JsonOperations::get_str_value(const std::string &json_str, const std::string &key)
    {
        std::regex reg("\"" + key + "\":\\s\"([^\"]+)\"");
        std::smatch match;
        std::regex_search(json_str, match, reg);
        if (!match.empty())
        {
            return match[1];
        }
        return "";
    }

    Transaction JsonOperations::get_transaction(const std::string &json_str)
    {
        Transaction transaction;

        transaction.timestamp = JsonOperations::get_unsigned_long_int_value(json_str, "timestamp");
        transaction.sender_signature = JsonOperations::get_str_value(json_str, "senderSignature");
        transaction.sender_public_key = JsonOperations::get_str_value(json_str, "senderPublicKey");
        transaction.hash = JsonOperations::get_str_value(json_str, "hash");
        transaction.gas = JsonOperations::get_int_value(json_str, "gas");

        std::regex reg("\"txIns\": \\[([^\\]]+)\\]");
        std::smatch match;
        std::regex_search(json_str, match, reg);
        std::string tx_ins_json = match[1];

        std::vector<std::string> tx_in_jsons;
        std::regex reg2("(\\{[^}]+\\})");
        std::smatch match2;
        std::string strr = tx_ins_json;
        while (std::regex_search(strr, match2, reg2))
        {
            if (!match2.empty())
            {
                tx_in_jsons.push_back(match2[1]);
                strr = match2.suffix();
            }
        }

        for (const std::string &tx_in_json : tx_in_jsons)
        {
            TxIn tx_in;
            tx_in.block_id = JsonOperations::get_int_value(tx_in_json, "blockId");
            tx_in.transaction_hash = JsonOperations::get_str_value(tx_in_json, "transactionHash");
            tx_in.tx_out_index = JsonOperations::get_int_value(tx_in_json, "txOutIndex");
            transaction.tx_ins.push_back(tx_in);
        }

        std::regex reg4("\"txOuts\": \\[([^\\]]+)\\]");
        std::smatch match4;
        std::regex_search(json_str, match4, reg4);
        std::string tx_outs_json = match4[1];


        std::vector<std::string> tx_out_jsons;
        std::regex reg3("(\\{[^}]+\\})");
        std::smatch match3;
        strr = tx_outs_json;
        while (std::regex_search(strr, match3, reg3))
        {
            if (!match3.empty())
            {
                tx_out_jsons.push_back(match3[1]);
                strr = match3.suffix();
            }
        }

        for (const std::string &tx_out_json : tx_out_jsons)
        {
            TxOut tx_out;
            tx_out.index = JsonOperations::get_int_value(tx_out_json, "index");
            tx_out.amount = JsonOperations::get_unsigned_long_int_value(tx_out_json, "amount");
            tx_out.receiver_public_key = JsonOperations::get_str_value(tx_out_json, "receiverPublicKey");
            transaction.tx_outs.push_back(tx_out);
        }

        return transaction;
    }

    std::vector<TxInWithAmount> JsonOperations::get_array_of_tx_ins_with_amount(const std::string &json_str)
    {
        std::vector<TxInWithAmount> result;

        std::regex reg("\"txIn\": (\\{[^\\}]+\\})");

        std::vector<std::string> tx_in_jsons;

        std::smatch match;
        std::string strr = json_str;
        while (std::regex_search(strr, match, reg))
        {
            if (!match.empty())
            {
                tx_in_jsons.push_back(match[1]);
                strr = match.suffix();
            }
        }

        std::vector<TxIn> tx_ins;

        for (const std::string &tx_in_json : tx_in_jsons)
        {
            TxIn tx_in;
            tx_in.block_id = JsonOperations::get_int_value(tx_in_json, "blockId");
            tx_in.transaction_hash = JsonOperations::get_str_value(tx_in_json, "transactionHash");
            tx_in.tx_out_index = JsonOperations::get_int_value(tx_in_json, "txOutIndex");
            tx_ins.push_back(tx_in);
        }

        std::vector<unsigned long int> amounts;
        std::regex reg2("\"amount\":\\s(-*\\d+)");
        std::smatch match2;
        strr = json_str;
        while (std::regex_search(strr, match2, reg2))
        {
            if (!match2.empty())
            {
                amounts.push_back(std::stoul(match2[1]));
                strr = match2.suffix();
            }
        }

        for (int i = 0; i < tx_ins.size(); i++)
        {
            TxInWithAmount res;
            res.amount = amounts[i];
            res.tx_in = tx_ins[i];
            result.push_back(res);
        }

        return result;
    }

    std::string JsonOperations::transaction_to_json(const Transaction &transaction)
    {
        std::string json_str("{ ");
        json_str += "\"timestamp\": ";
        json_str += std::to_string(transaction.timestamp) + ", ";
        json_str += "\"senderSignature\": ";
        json_str += "\"" + transaction.sender_signature + "\", ";
        json_str += "\"senderPublicKey\": ";
        json_str += "\"" + transaction.sender_public_key + "\", ";
        json_str += "\"hash\": ";
        json_str += "\"" + transaction.hash + "\", ";
        json_str += "\"gas\": ";
        json_str += std::to_string(transaction.gas) + ", ";

        json_str += "\"txIns\": [ ";

        int i = 0;
        for (const TxIn &tx_in : transaction.tx_ins)
        {
            json_str += JsonOperations::tx_in_to_json(tx_in);

            if (++i != transaction.tx_ins.size())
            {
                json_str += ", ";
            }
        }

        json_str += " ], \"txOuts\": [";

        i = 0;
        for (const TxIn &tx_in : transaction.tx_ins)
        {
            json_str += JsonOperations::tx_in_to_json(tx_in);

            if (++i != transaction.tx_ins.size())
            {
                json_str += ", ";
            }
        }

        json_str += " ] }";

        return json_str;
    }

    std::string JsonOperations::tx_in_to_json(const TxIn &tx_in)
    {
        std::string json_str("{ ");
        json_str += "\"blockId\": ";
        json_str += std::to_string(tx_in.block_id) + ", ";
        json_str += "\"txOutIndex\": ";
        json_str += std::to_string(tx_in.tx_out_index) + ", ";
        json_str += "\"transactionHash\": ";
        json_str += "\"" + tx_in.transaction_hash + "\" }";

        return json_str;
    }

    std::string JsonOperations::tx_out_to_json(const TxOut &tx_out)
    {
        std::string json_str("{ ");
        json_str += "\"index\": ";
        json_str += std::to_string(tx_out.index) + ", ";
        json_str += "\"amount\": ";
        json_str += std::to_string(tx_out.amount) + ", ";
        json_str += "\"receiverPublicKey\": ";
        json_str += "\"" + tx_out.receiver_public_key + "\" }";

        return json_str;
    }

}