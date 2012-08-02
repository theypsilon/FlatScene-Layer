#ifndef FS_XML_HELPER__
#define FS_XML_HELPER__

#include "Exception.h"
#include <limits>
#include <string>

namespace FlatScene { namespace Util { namespace XML { namespace Tiny {

    template <class XMLElement>
    bool checkAttr(const XMLElement& el,const char *const name, const char *const value = nullptr, bool equals = true) {
        if (!name || strcmp(name,"")==0) throw Exception("no attribute name supplied",__LINE__); //FIXME static_assert?
        auto attr = el.Attribute(name);
        if (!attr) return false;
        if (value && ( ( equals && strcmp(attr,value)!=0) || 
                       (!equals && strcmp(attr,value)==0) )) 
                       return false;
        return true;
    }

    template <class XMLElement>
    void ensureAttr(const XMLElement& el,const char *const name, const char *const value = nullptr, bool equals = true) {
        if (!checkAttr(el,name,value,equals))  throw Exception( std::string("invalid attribute '")+
                (name?name:"nullptr")+"' with value '"+ (value?value:"nullptr")+"' equals="+(equals?"true":"false"));
    }

    inline void processBadResult(int cod,const std::string& attr,const std::type_info& type) {
#if defined(TINYXML_INCLUDED)
        switch(cod) {
            case TIXML_NO_ATTRIBUTE:
                throw Exception("Attribute '"+attr+"' doesn't exist",__LINE__);
            break; case TIXML_WRONG_TYPE:
                throw Exception("Attribute '"+attr+"' is not a "+type.name(),__LINE__);
            break; default:
                throw Exception("Attribute '"+attr+"' unknown "+type.name(),__LINE__);
            break;
        }
#else
#define TIXML_SUCCESS 0
#endif
    }

    template <class XMLElement>
    int intFromAttr(const XMLElement& el,const char *const name) {
        int ret, cod = el.QueryIntAttribute(name,&ret);
        if (cod != TIXML_SUCCESS)
            processBadResult(cod,name,typeid(int));
        return ret;
    }

    template<typename T, class XMLElement> 
    T valFromAttr(const XMLElement& el,const std::string& name) {
        T ret;
        int cod = el.QueryValueAttribute(name,&ret);
        if (cod != TIXML_SUCCESS)
            processBadResult(cod,name,typeid(T));
        return ret;
    }

    template<typename T, class XMLElement> 
    T numFromAttr(const XMLElement& el,const std::string& name,
            T min=std::numeric_limits<T>::min(), T max=std::numeric_limits<T>::max()) {
        T ret = valFromAttr<T>(el,name);
        if (ret < min || ret > max)
           throw Exception("attribute '"+name+"' not in "+typeid(T).name()+" range",__LINE__);
        return ret;
    }

#if !defined(TINYXML_INCLUDED)
#undef TIXML_SUCCESS
#endif

}}}} // FlatScene::Util::xml::tiny

#endif // FS_XML_HELPER__
