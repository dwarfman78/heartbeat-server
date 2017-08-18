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

    static std::map<std::string, xmlrpc_c::value> createRecordForQuery(const std::string& idRecord)
    {
        std::map<std::string, xmlrpc_c::value>  returnValue;

        returnValue["id"] = xmlrpc_c::value_string(idRecord);

        return returnValue;
    }

private:
    XmlRpcUtils(){}
};
#endif
