#include "NetworkManager.hpp"

NetworkManager::NetworkManager(std::string password, int port) : mPassword(password)
{
    mSocket.bind(port);
}

bool NetworkManager::getNextPacket(std::tuple<std::string, sf::IpAddress, unsigned short> &nextTuple)
{
    bool returnValue{false};

    sf::Packet packet;

    sf::IpAddress senderAddress;

    unsigned short senderPort;

    std::string subDomain;

    std::string hash;

    mSocket.receive(packet, senderAddress, senderPort);

    if(packet >> subDomain >> hash)
    {
        returnValue = checkHash(hash,subDomain,senderAddress);

        if(returnValue)
        {
            nextTuple = std::make_tuple(subDomain,senderAddress,senderPort);
        }
    }

    return returnValue;
}

bool NetworkManager::checkHash(const std::string &hash, const std::string &subDomain, const sf::IpAddress &sender)
{
    bool returnValue{false};

    std::string newHash = mSha256(subDomain + sender.toString()+mPassword);

    std::cout << "checking incoming hash " << hash << " over local hash : " << newHash << std::endl;

    returnValue = (newHash.compare(hash) == 0);

    return returnValue;
}
