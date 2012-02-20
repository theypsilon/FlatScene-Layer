#ifndef __FS_XML_HELPER__
#define __FS_XML_HELPER__

#include "FSException.h"
#include "FSparserXML.h"
#include <string>

namespace fs { namespace intern { namespace xml {

    bool checkAttr(const TiXmlElement& el,const char *const name, const char *const value = nullptr, bool equals = true) {
        if (!name) throw FSException("no attribute name supplied",__LINE__);
        auto attr = el.Attribute(name);
        if (!attr) return false;
        if (value && ( ( equals && strcmp(attr,value)!=0) || 
                       (!equals && strcmp(attr,value)==0) )) 
                       return false;
        return true;
    }

    void ensureAttr(const TiXmlElement& el,const char *const name, const char *const value = nullptr, bool equals = true) {
        if (!checkAttr(el,name,value,equals))  throw FSException( std::string("invalid attribute '")+
                (name?name:"nullptr")+"' with value '"+ (value?value:"nullptr")+"' equals="+(equals?"true":"false"));
    }

    int intFromAttr(const TiXmlElement& el,const char *const name) {
        ensureAttr(el,name);
        int ret;
        el.QueryIntAttribute(name,&ret);
        return ret;
    }

    int intFromAttr(const TiXmlElement& el,const char *const name,int min, int max) {
        int ret = intFromAttr(el,name);
        if (ret < min || ret > max) 
            throw FSException(std::string("int '")+name+"' not in range",__LINE__);
        return ret;
    }

}}}

#endif
