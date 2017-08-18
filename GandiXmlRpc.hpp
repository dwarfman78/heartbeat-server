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

#ifndef GandiXmlRpc_hpp
#define GandiXmlRpc_hpp
#include <iostream>
#include <vector>
#include <xmlrpc-c/client_simple.hpp>
#include <SFML/System/NonCopyable.hpp>

/*
 * This class takes care of calling gandi's xmlRpc API.
 */
class GandiXmlRpc : public sf::NonCopyable
{
public:

    /*
    * Constructor.
    *
    * @param apiUrl : URL of the Gandi xmlRpc API.
    * @param apiKey : Private key to the api.
    */
    GandiXmlRpc(const std::string& apiUrl, const std::string& apiKey);

    /*
     * Main method, update dns dynamically if ipAddress is different from the current active ip for that sub-domain.
     *
     * @param domain : Domain to be updated.
     * @param subDomain : Subdomain to be updated.
     * @param ipAddress : ip address to set.
     */
    void updateDns(const std::string& domain, const std::string& subDomain, const std::string& ipAddress);

    /*
     * Get the id of the active zone for the given domain name.
     *
     * @param domainName : name of the tested domain.
     *
     * @return the id of the current active zone.
     */
    int getZoneId(const std::string& domainName);

    /*
    * Tells if the ip associated with the record named recordName of the given zone
    * is equal or different than the ipAddress parameter.
    *
    * @param zoneId zone id.
    * @param recordName name of the record to be checked inside the zone.
    * @param ipAddress ip address to be tested for changing.
    *
    * @return true if ip has changed false otherwise.
    */
    bool hasIpChanged(int zoneId, const std::string& recordName, const std::string& ipAddress);

    /*
     * Retrieve a record id from its zone and its name.
     *
     * @param zoneId : zone identifier.
     * @param zoneVersion : zone version.
     * @param recordName : name of the record
     *
     * @return -1 if not found or the record id.
     */
     std::string getRecordId(int zoneId, int zoneVersion, const std::string& recordName);

    /*
     * Retrieve record list from a zone identified by its id (active zone by default).
     *
     * @param zoneId : zone identifier.
     * @param versionId : zone version (default = 0).
     *
     * @return list of records.
     */
    std::vector<xmlrpc_c::value> getRecordList(int zoneId, int versionId = 0);

    /*
     * Retrieve a record from an identified zone from its name.
     *
     * @param zoneId : zone identifier.
     * @param zoneVersion : zone version (default = 0 = active zone)
     * @param recordName : record name.
     *
     * @return the record or empty structure if not found.
     */
    std::map<std::string,xmlrpc_c::value> getRecordByName(int zoneId, const std::string& recordName, int zoneVersion = 0);

    /*
     * Create a new version of the given zone and return the new zone id.
     *
     * @param zoneId : current zone id to clone.
     *
     * @return new zone id.
     */
    int createNewZoneVersion(int zoneId);

    /*
     * Update a record.
     *
     * @param zoneId : current zone id.
     * @param versionId : current zone version id.
     * @param recordToUpdate : record description to be updated.
     * @param zoneRecord : new record value.
     */
    void updateRecord(int zoneId, int versionId, std::map<std::string,xmlrpc_c::value>& recordToUpdate, std::map<std::string,xmlrpc_c::value>& zoneRecord);

    /*
     * Define which version is active for a given zone.
     *
     * @param zoneId : zone identifier.
     * @param versionId : version identifier.
     */
    void setZoneVersion(int zoneId, int versionId);

private:
    /*
     * Simple XmlRpc Client.
     */
    xmlrpc_c::clientSimple mXmlRpcClient;

    /*
     * Service url.
     */
    std::string mApiUrl;

    /*
     * API key.
     */
    std::string mApiKey;
};
#endif
