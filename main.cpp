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

        // strongPassword
        std::string password = argv[5];

        NetworkManager networkManager(password, atoi(port.c_str()));

        GandiXmlRpc service(apiUrl, apiKey);

        std::tuple<std::string, sf::IpAddress, unsigned short> nextTuple;

        while (true)
        {
            try
            {
                // Will check hash and packet integrity
                if (networkManager.getNextPacket(nextTuple))
                {
                    std::cout << "Receiving packet from : " << std::get<1>(nextTuple) << std::endl;

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
        std::cout << "invalid arguments, expecting : API_URL API_KEY DOMAIN_NAME LISTENING_PORT PASSWORD" << std::endl;
        return 1;
    }
}
