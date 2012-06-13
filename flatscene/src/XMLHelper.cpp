#include "XMLHelper.h"

namespace flatscene { namespace intern { namespace xml {

    bool checkAttr(const TiXmlElement& el,const char *const name, const char *const value, bool equals) {
        if (!name || strcmp(name,"")==0) throw Exception("no attribute name supplied",__LINE__); //FIXME static_assert?
        auto attr = el.Attribute(name);
        if (!attr) return false;
        if (value && ( ( equals && strcmp(attr,value)!=0) || 
                       (!equals && strcmp(attr,value)==0) )) 
                       return false;
        return true;
    }

    void ensureAttr(const TiXmlElement& el,const char *const name, const char *const value, bool equals) {
        if (!checkAttr(el,name,value,equals))  throw Exception( std::string("invalid attribute '")+
                (name?name:"nullptr")+"' with value '"+ (value?value:"nullptr")+"' equals="+(equals?"true":"false"));
    }

    void processBadResult(int cod,const std::string& attr,const std::type_info& type) {
        switch(cod) {
            case TIXML_NO_ATTRIBUTE: 
                throw Exception("Attribute '"+attr+"' doesn't exist",__LINE__); 
            break; case TIXML_WRONG_TYPE:                
                throw Exception("Attribute '"+attr+"' is not a "+type.name(),__LINE__); 
            break; default: 
                throw Exception("Attribute '"+attr+"' unknown "+type.name(),__LINE__); 
            break;
        }
    }

    int intFromAttr(const TiXmlElement& el,const char *const name) {
        int ret, cod = el.QueryIntAttribute(name,&ret);
        if (cod != TIXML_SUCCESS)
            processBadResult(cod,name,typeid(int));
        return ret;
    }

}}} // namespace flatscene { namespace intern { namespace xml
