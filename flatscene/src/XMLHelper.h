#ifndef __FS_XML_HELPER__
#define __FS_XML_HELPER__

#include "FSException.h"
#include "FSparserXML.h"
#include <string>
#include <limits>

namespace fs { namespace intern { namespace xml {

    bool checkAttr(const TiXmlElement& el,const char *const name, const char *const value = nullptr, bool equals = true) {
        if (!name || strcmp(name,"")==0) throw FSException("no attribute name supplied",__LINE__); //FIXME static_assert?
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

    void processBadResult(int cod,const std::string& attr,const std::type_info& type) {
        switch(cod) {
            case TIXML_NO_ATTRIBUTE: 
                throw FSException("Attribute '"+attr+"' doesn't exist",__LINE__); 
            break; case TIXML_WRONG_TYPE:                
                throw FSException("Attribute '"+attr+"' is not a "+type.name(),__LINE__); 
            break; default: 
                throw FSException("Attribute '"+attr+"' unknown "+type.name(),__LINE__); 
            break;
        }
    }

    int intFromAttr(const TiXmlElement& el,const char *const name) {
        int ret, cod = el.QueryIntAttribute(name,&ret);
        if (cod != TIXML_SUCCESS)
            processBadResult(cod,name,typeid(int));
        return ret;
    }

    template<typename T> 
    T valFromAttr(const TiXmlElement& el,const std::string& name) {
        T ret;
        int cod = el.QueryValueAttribute(name,&ret);
        if (cod != TIXML_SUCCESS)
            processBadResult(cod,name,typeid(T));
        return ret;
    }

    template<typename T> 
    T numFromAttr(const TiXmlElement& el,const std::string& name,
            T min=std::numeric_limits<T>::min(), T max=std::numeric_limits<T>::max()) {
        T ret = valFromAttr<T>(el,name);
        if (ret < min || ret > max)
           throw FSException("attribute '"+name+"' not in "+typeid(T).name()+" range",__LINE__);
        return ret;
    }

}}}

#endif
