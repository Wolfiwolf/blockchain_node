#ifndef BLOCKCHAIN_CONTACT_INFO_H
#define BLOCKCHAIN_CONTACT_INFO_H

#include <string>

namespace BlockchainNode
{

    struct BlockchainNodeContactInfo
    {
        std::string ip_address;
        int port;
    };

}

#endif