#include "GandiXmlRpc.hpp"
#include "XmlRpcUtils.hpp"

GandiXmlRpc::GandiXmlRpc(const std::string &apiUrl, const std::string &apiKey) : mApiUrl(apiUrl), mApiKey(apiKey)
{
}

int GandiXmlRpc::getZoneId(const std::string &domainName)
{
    xmlrpc_c::value result;
    /*
    *
    * Get domain information.
    * Parameters:
    * apikey (string) – API connection key
    * domain (string) – Name of the domain
    * Returns:
    * struct – Domain information
    *
    */
    mXmlRpcClient.call(mApiUrl, "domain.info", "ss", &result, mApiKey.c_str(), domainName.c_str());

    auto data = XmlRpcUtils::toMap(result);

    return XmlRpcUtils::toInt(data.at("zone_id"));
}

bool GandiXmlRpc::hasIpChanged(int zoneId, const std::string &recordName, const std::string &ipAddress)
{
    bool returnValue{false};

    // find record by its name.
    auto record = getRecordByName(zoneId,recordName);

    if(record.size()!=0)
    {
        auto it = record.find("value");

        if(it!=record.end())
        {
            // if record contains the "value" field, we compare the actual value with ipAddress.
            returnValue = (XmlRpcUtils::toString(it->second).compare(ipAddress) != 0);
        }
    }

    return returnValue;
}

int GandiXmlRpc::createNewZoneVersion(int zoneId)
{
    int returnValue{-1};

    xmlrpc_c::value result;

    /*
     * Create a new version from another version. This will duplicate the version’s records.
     * Note that cloned records will have new identifiers.
     * Parameters:
     *
     * apikey (string) – API connection key
     * zone_id (int) – id of the Zone
     * version_id (int) – version_id to start from, current version is used by default
     *
     * Returns:
     * int – the created version number
     */
    mXmlRpcClient.call(mApiUrl,"domain.zone.version.new","si",&result,mApiKey.c_str(),zoneId);

    returnValue = XmlRpcUtils::toInt(result);

    return returnValue;
}

void GandiXmlRpc::updateRecord(int zoneId, int versionId, std::map<std::string, xmlrpc_c::value> &recordToUpdate, std::map<std::string, xmlrpc_c::value> &zoneRecord)
{
    xmlrpc_c::value result;

    xmlrpc_c::paramList params;

    params.add(xmlrpc_c::value_string(mApiKey));

    params.add(xmlrpc_c::value_int(zoneId));

    params.add(xmlrpc_c::value_int(versionId));

    params.add(xmlrpc_c::value_struct(recordToUpdate));

    params.add(xmlrpc_c::value_struct(zoneRecord));

    /*
     * Update one record by its id.
     *
     * Parameters:
     * apikey (string) – API connection key
     * zone_id (int) – id of the Zone
     * version_id (int) – version_id to update records from
     * opts (struct) – record to update
     * params (struct) – new specification for record
     *        Returns:
     * list of struct – the updated record
     */

    mXmlRpcClient.call(mApiUrl,"domain.zone.record.update",params,&result);

}

void GandiXmlRpc::setZoneVersion(int zoneId, int versionId)
{
    xmlrpc_c::value result;

    /*
     * Sets the records for a zone/version
     * Parameters:
     * apikey (string) – API connection key
     * zone_id (int) – id of the Zone
     * version_id (int) – version_id to list
     * params (struct) – the record definition
     * Returns:
     * struct – The records
     */
    mXmlRpcClient.call(mApiUrl,"domain.zone.version.set","sii",&result,mApiKey.c_str(),zoneId,versionId);
}

int GandiXmlRpc::getRecordId(int zoneId, int zoneVersion, const std::string &recordName)
{
    int returnValue{-1};

    // find record by its name.
    auto record = getRecordByName(zoneId, recordName, zoneVersion);

    if(record.size()!=0)
    {
        auto it = record.find("id");

        if(it!=record.end())
        {
            // if record found and contains the "id" field, we return the value of the field.
            returnValue = XmlRpcUtils::toInt(it->second);
        }
    }

    return returnValue;
}

std::vector<xmlrpc_c::value> GandiXmlRpc::getRecordList(int zoneId, int versionId)
{
    xmlrpc_c::value result;

    /*
     * List records of a version of a DNS zone.
     *
     * Parameters:
     * apikey (string) – API connection key
     * zone_id (int) – ID of the Zone
     * version_id (int) – version_id to list, 0 for active version
     * opts (struct) – optional filters
     * Returns:
     * struct – a list of records
     */
    mXmlRpcClient.call(mApiUrl, "domain.zone.record.list", "sii", &result, mApiKey.c_str(), zoneId, versionId);

    return XmlRpcUtils::toArray(result);
}

std::map<std::string, xmlrpc_c::value> GandiXmlRpc::getRecordByName(int zoneId, const std::string &recordName, int zoneVersion)
{
    bool found{false};

    std::map<std::string, xmlrpc_c::value> returnValue;

    // Retrieval of all records.
    auto arrayData = getRecordList(zoneId,zoneVersion);

    auto it = arrayData.begin();

    while (it != arrayData.end() && !found)
    {
        // Check one record at a time.
        auto mapData = XmlRpcUtils::toMap(*it);

        if (XmlRpcUtils::toString(mapData.at("name")).compare(recordName) == 0)
        {
            // we check if the "name" field is equals to the recordName parameter.
            // if so, we return the current record and break the loop.
            returnValue = mapData;
            found=true;
        }

        ++it;
    }

    return returnValue;
}

void GandiXmlRpc::updateDns(const std::string &domain, const std::string &subDomain, const std::string &ipAddress)
{
    std::cout << "updating DNS for domain : " << domain << " sub-domain : " << subDomain << " ip address : " << ipAddress << std::endl;

    int zoneId = getZoneId(domain);

    std::cout << "testing ip for zoneId : " << zoneId << std::endl;

    if(hasIpChanged(zoneId, subDomain, ipAddress))
    {
        std::cout << "ip changed, updating..." << std::endl;

        int newZoneVersion = createNewZoneVersion(zoneId);

        int recordId = getRecordId(zoneId,newZoneVersion,subDomain);

        std::cout << " updating record id " << recordId << std::endl;

        auto recordToUpdate = XmlRpcUtils::createRecordForQuery(recordId);

        auto zoneRecord = XmlRpcUtils::createRecord(subDomain, "A", ipAddress, 300);

        updateRecord(zoneId, newZoneVersion, recordToUpdate, zoneRecord);

        std::cout << " setting new active version " << newZoneVersion << std::endl;

        setZoneVersion(zoneId, newZoneVersion);

        std::cout << " done." << std::endl;

    }
    else
    {
        std::cout << "No ip change detected... nothing to do." << std::endl;
    }
}
