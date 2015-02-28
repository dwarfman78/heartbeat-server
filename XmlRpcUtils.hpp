#ifndef XmlRpcUtils_hpp
#define XmlRpcUtils_hpp

#include <xmlrpc-c/base.hpp>

/*
 * Utilitary class for xmlrpc_c type handling.
 */
class XmlRpcUtils
{

public:
    static std::string toString(xmlrpc_c::value& value){return xmlrpc_c::value_string(value);}
    static std::map<std::string, xmlrpc_c::value> toMap(xmlrpc_c::value& value){return xmlrpc_c::value_struct(value);}
    static int toInt(xmlrpc_c::value& value){return xmlrpc_c::value_int(value);}
    static std::vector<xmlrpc_c::value> toArray(xmlrpc_c::value& value){return xmlrpc_c::value_array(value).cvalue();}

    static std::map<std::string, xmlrpc_c::value> createRecord(const std::string& name, const std::string& type, const std::string& value, int ttl)
    {
        std::map<std::string, xmlrpc_c::value>  returnValue;

        returnValue["name"] = xmlrpc_c::value_string(name);
        returnValue["type"] = xmlrpc_c::value_string(type);
        returnValue["value"] = xmlrpc_c::value_string(value);
        returnValue["ttl"] = xmlrpc_c::value_int(ttl);

        return returnValue;
    }

    static std::map<std::string, xmlrpc_c::value> createRecordForQuery(int idRecord)
    {
        std::map<std::string, xmlrpc_c::value>  returnValue;

        returnValue["id"] = xmlrpc_c::value_int(idRecord);

        return returnValue;
    }

private:
    XmlRpcUtils(){}
};
#endif