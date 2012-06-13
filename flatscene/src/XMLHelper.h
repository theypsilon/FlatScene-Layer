#ifndef FS_XML_HELPER__
#define FS_XML_HELPER__

#include "FSException.h"
#include "FSparserXML.h"
#include <string>
#include <limits>

namespace flatscene { namespace intern { namespace xml {

    bool checkAttr(const TiXmlElement& el,const char *const name, const char *const value = nullptr, bool equals = true);

    void ensureAttr(const TiXmlElement& el,const char *const name, const char *const value = nullptr, bool equals = true);

    void processBadResult(int cod,const std::string& attr,const std::type_info& type);

    int intFromAttr(const TiXmlElement& el,const char *const name);

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
           throw Exception("attribute '"+name+"' not in "+typeid(T).name()+" range",__LINE__);
        return ret;
    }

}}} // namespace flatscene { namespace intern { namespace xml

#endif // FS_XML_HELPER__
