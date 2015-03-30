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

#include <SFML/Network.hpp>
#include <iostream>
#include "GandiXmlRpc.hpp"
#include "NetworkManager.hpp"

int main(int argc, char *argv[])
{

    if (argc == 6)
    {
        // https://rpc.gandi.net/xmlrpc/
        std::string apiUrl = argv[1];

        // YoUrGaNdIApiKEy
        std::string apiKey = argv[2];

        // myDomain.fr
        std::string domain = argv[3];

        // listening port
        std::string port = argv[4];

        // password file
        std::string passwordFile = argv[5];

        NetworkManager networkManager(passwordFile, atoi(port.c_str()));

        GandiXmlRpc service(apiUrl, apiKey);

        std::tuple<std::string, sf::IpAddress, unsigned short> nextTuple;
        
        std::cout << "Hello, launching heartbeat-server (1.1)" << std::endl;

        while (true)
        {
            try
            {
                // Will check hash and packet integrity
                if (networkManager.getNextPacket(nextTuple))
                {
                    // nextTuple[0] is subDomain, nextTuple[1] is current client ipAddress
                    service.updateDns(domain, std::get<0>(nextTuple), std::get<1>(nextTuple).toString());
                }
                else
                {
                    std::cout << "Error reading packet" << std::endl;
                }
            } catch (std::exception &exe)
            {
                std::cout << "Exception occured" << exe.what() << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Invalid arguments, expecting : API_URL API_KEY DOMAIN_NAME LISTENING_PORT PASSWORD_FILE" << std::endl;
        return 1;
    }
}
