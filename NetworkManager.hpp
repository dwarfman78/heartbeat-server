#ifndef NetworkManager_hpp
#define NetworkManager_hpp

#include <SFML/Network.hpp>
#include <iostream>
#include "sha256.h"

/*
 * Network managing.
 */
class NetworkManager
{
public:
    /*
     * Constructor.
     *
     * @param port listening port.
     */
    explicit NetworkManager(std::string password, int port = 5009);

    /*
     * Blocking method, waits for the next packet and populates nextTuple with incoming data.
     * Also performs an hash verification.
     *
     * @param nextTuple tuple populated from the incoming packet if it is valid.
     *
     * @return true if the incoming packet is valid, false otherwise.
     */
    bool getNextPacket(std::tuple<std::string,sf::IpAddress,unsigned short>& nextTuple);

private:
    /*
     * Password.
     */
    std::string mPassword;

    /*
     * Udp socket.
     */
    sf::UdpSocket mSocket;

    /*
     * Sha.
     */
    SHA256 mSha256;

    /*
     * Check incoming packet hash by calculating a new hash from incoming data.
     *
     * @param hash : incoming hash.
     * @param subDomain : incoming subDomain.
     * @param sender : sender Ip.
     *
     * @return true if new calculated hash equals received hash.
     */
    bool checkHash(const std::string& hash,const std::string& subDomain, const sf::IpAddress& sender);

};

#endif

