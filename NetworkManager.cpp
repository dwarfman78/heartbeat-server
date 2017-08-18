/*Copyright (C) <2015> <Fabien LE CORRE (dwarf@dwarfman.fr)>

This software is provided 'as-is', without any express or implied
        warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
        including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.*/

#include "NetworkManager.hpp"

NetworkManager::NetworkManager(const std::string& passwordFile, int port)
{
   mSocket.bind(port);
    
   loadPasswords(passwordFile);
}

void NetworkManager::loadPasswords(const std::string& passwordFile)
{
    std::ifstream cFile (passwordFile);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line))
	{
            line.erase(remove_if(line.begin(), line.end(), isspace),
                                 line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
	    
	    mPasswords[name]=value;
        }
        cFile.close();
    }
    else
        std::cout << "Unable to open config file." << '\n';
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
    
    std::string timestamp = getTimestamp();
    
    std::cout << "Receiving heartbeat with : \n\t Subdomain : "<< subDomain <<"\n\t Sender : " << sender.toString() << "\n\t Timestamp : " << timestamp << std::endl;
    
    std::string newHash = mSha256(subDomain + sender.toString()+mPasswords[subDomain]+timestamp);

    std::cout << "Checking Hash  : \n\t Received : " << hash << " \n\t Local : " << newHash << std::endl;

    returnValue = (newHash.compare(hash) == 0);

    return returnValue;
}
std::string NetworkManager::getTimestamp() const
{
    time_t now;
    time(&now);
    struct tm* ptm;
        
    ptm = gmtime(&now);
        
    return std::to_string(ptm->tm_yday);
}
