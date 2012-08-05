#ifndef FS_SPRITESET_IMPL__
#define FS_SPRITESET_IMPL__

#include "SpriteSet.h"
#include "Exception.h"
#include "Algorithm.h"
#include "Traits.h"
#include "parserXML.h"
#include "Library.h"

#include <algorithm>
#include <iostream>

#include "TinyXMLHelper.h"
#include "ImageFactory.h"
#include "SpriteResource.h"
#include "RefCountMemoryPolicyImpl.h"

#include "GraphicResourceDescriptor.h"

namespace FlatScene {

using namespace Util::XML::Tiny;

class SpritesetResource {
public:

    typedef std::vector<Sprite> SpriteCollection;

    SpritesetResource(std::string c, GraphicMode mode) 
        : _mode(mode), _name(std::move(c)) {
            loadChipset(_name,mode);
    }

    GraphicMode getMode() const {
        return _mode;
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

    void add ( Sprite pspt ) {
        //_sprites.push_back ( pspt ) ;
    }

    const Sprite* get ( unsigned int n ) const {
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

    SpriteCollection _sprites;
    std::string      _name;
    GraphicMode      _mode;

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

    void loadChipset(const std::string& c,GraphicMode mode) {
        auto fGRDsChipset = loadGRDandChipset(getNameFile(c));

        loadAllSprites(fGRDsChipset.first,toCRef(fGRDsChipset.second),mode);

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
    void loadAllSprites(const GRD& grd, const T& chipset, GraphicMode mode) {
        unsigned int w = getWidth(chipset), h = getHeight(chipset);
        if (w / grd._cellwidth <= 0 || w % grd._cellwidth != 0)
            throw Exception("the grd file doesn't fit with the chipset",__LINE__);

        RectangleImage  src = {0,0,0,0};
        unsigned int    i = 0;
        for (const auto& img : grd._images) {

            src.w = src.x + img.dim.x;
            src.h = src.y + img.dim.y;

            Sprite spt(createResource<SpriteResource>(src,chipset,mode,grd,i++));

            spt.setName(img.name);

            _sprites.push_back(std::move(spt));

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
