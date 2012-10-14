#ifndef FS_SPRITESET_IMPL__
#define FS_SPRITESET_IMPL__

#include "ImageSet.h"
#include "Exception.h"
#include "Algorithm.h"
#include "Traits.h"
#include "parserXML.h"
#include "Library.h"

#include <algorithm>
#include <iostream>

#include "TinyXMLHelper.h"
#include "CanvasResourceFactory.h"
#include "SpriteResource.h"
#include "RefCountMemoryPolicyImpl.h"

#include "GraphicResourceDescriptor.h"

namespace FlatScene {

namespace detail {

    template <class Res> struct to_graphic_mode {
        static const GraphicMode value = ONLY_GPU;
    };

    template <> struct to_graphic_mode<SoftwareSprite> {
        static const GraphicMode value = ONLY_CPU;
    };

    template <> struct to_graphic_mode<SoftwareCanvas> {
        static const GraphicMode value = ONLY_CPU;
    };
} // detail

using namespace Util::XML::Tiny;

template <class Res>
class ImageSetResource {
public:

    template <class T>
    static ImageSetResource<Res>*const create(T&& c) {
        typedef RefCountMemoryPolicy<ImageSetResource<Res>> MemoryPolicyType;
        for(auto& set : MemoryPolicyType::getCounts()) {
            if (set.first->getName() == c)
                return set.first;
        }
        return new ImageSetResource<Res>(std::forward<T>(c));
    }

    template <class T>
    ImageSetResource(T&& c) 
        : _name(std::forward<T>(c)) {
            loadChipset(_name);
    }

    const std::string& getName() const {
        return _name;
    }

    bool setName(std::string& name) {
        if (this->_name == "") {
            this->_name = name;
            return true;
        }
        return false;
    }

    void add ( Res pspt ) {
        //_sprites.push_back ( pspt ) ;
    }

    const std::vector<Res>& get() const {
        return _sprites;
    }

    const Res* get ( unsigned int n ) const {
        if ( n < _sprites.size()) {
            return &_sprites.at(n);
        } else {
            return nullptr;
        }
    }

    int size() const {
        return _sprites.size();
    }

private:

    std::vector<Res> _sprites;
    std::string         _name;

    typedef decltype(IMGLoadOrThrow("")) csType;

    std::pair<GRD,csType> loadGRDandChipset(const std::pair<std::string, std::string>& pair) {
        const std::string& name = pair.first;
        const std::string& type = pair.second;
        try {
            GRD grd(name + ".grd");
            auto cs = IMGLoadOrThrow(grd.getGraphicFile());
            return std::make_pair(std::move(grd),std::move(cs));
        } catch(DocIsNotLoadedException&) {
            std::string graphicFile = name +(
                type != ".grd"? 
                     isValidBitmapExtension(type)? type : throw Exception("graphic bitmap format not valid") 
                     : ".png"
            );
            auto cs = IMGLoadOrThrow(graphicFile);
            return std::make_pair(
                GRD(getWidth(toCRef(cs)), getHeight(toCRef(cs)), std::move(graphicFile)),
                std::move(cs)
            );
        }
    }

    void loadChipset(const std::string& c) {
        auto fGRDsChipset = loadGRDandChipset(getNameFile(c));

        loadAllSprites(fGRDsChipset.first,toCRef(fGRDsChipset.second));

        IMGFreeOrThrow(fGRDsChipset.second);
    }

    bool isValidBitmapExtension(const std::string& bitmap) {
        const char* extensions[] = { ".png", ".bmp", ".jpg" };
        return binary_search(extensions,bitmap);
    }

    std::pair<std::string, std::string> getNameFile(const std::string& str) {
        auto res = str.end();
        for(auto it = str.begin(); it != str.end() ; ++it)
            if ('.' == *it)
                res = it;

        bool foundSomething = res != str.end();
        std::string namefile = foundSomething? std::string(str.begin(),res) : str;
        std::string tipefile = foundSomething? std::string(res,str.end())   : ".grd";

        return std::make_pair(namefile,tipefile);

    }

    template <typename T>
    void loadAllSprites(const GRD& grd, const T& chipset) {
        unsigned int w = getWidth(chipset), h = getHeight(chipset);
        if (w / grd._cellwidth <= 0 || w % grd._cellwidth != 0)
            throw Exception("the grd file doesn't fit with the chipset",__LINE__);

        RectangleImage  src = {0,0,0,0};
        unsigned int    i = 0;
        for (const auto& img : grd._images) {

            src.w = src.x + img.dim.x;
            src.h = src.y + img.dim.y;

            auto r = CanvasResource::create<typename Res::ResourceType>(src,chipset,grd,i++);
            r->applyMetadata(img);

            _sprites.push_back(Res(r));

            src.x += grd._cellwidth;
            if (src.x + grd._cellwidth > (unsigned int) w) {
                src.x = 0;
                src.y += grd._cellheight;
                if (src.y + grd._cellheight > (unsigned int) h)
                    throw Exception("the grd doesn't fit with the chipset",__LINE__);
            }
        }
    }

};

} // flatscene

#endif // FS_SPRITESET_IMPL__
