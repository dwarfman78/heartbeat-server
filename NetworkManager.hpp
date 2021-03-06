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

#ifndef NetworkManager_hpp
#define NetworkManager_hpp

#include <SFML/Network.hpp>
#include <iostream>
#include <ctime>
#include "sha256.h"
#include<fstream>
#include<algorithm>

/*
 * Network managing.
 */
class NetworkManager
{
public:
    /*
     * Constructor.
     *
     * @param passwordFile file containing passwords.
     * @param port listening port.
     */
    explicit NetworkManager(const std::string& passwordFile, int port = 5009);

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

    void loadPasswords(const std::string& passwordFile);
    /*
     * Password.
     */
    std::map<std::string,std::string> mPasswords;

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
    
    /*
     * Returns a timestamp corresponding
     * of the day in the current year.
     *
     * @return the day of the year.
     */
    std::string getTimestamp() const;

};

#endif

