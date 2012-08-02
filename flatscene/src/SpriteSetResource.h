#ifndef FS_SPRITESET_IMPL__
#define FS_SPRITESET_IMPL__

#include "SpriteSet.h"
#include "Exception.h"
#include "Algorithm.h"
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

    GraphicMode getMode() {
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

    DataGRD loadFileGRD(const std::string& grd_str, const SDL_Surface *const chipset) {
        try {
            return DataGRD(grd_str);
        } catch(DocIsNotLoadedException&) {
            if (!chipset) 
                throw Exception("grd file invalid and bitmap invalid",__LINE__);

            return DataGRD(chipset->w, chipset->h, grd_str);
        }
    }

    void loadChipset(const std::string& c,GraphicMode mode=ONLY_TEXTURE,std::string* cPrev=nullptr) {
        auto names = getNameFile(c);

        auto chipset = IMG_Load(names.second.c_str());
        if (!chipset)
            throw Exception("chipset couldn't load");

        auto grd = loadFileGRD(names.first,chipset);

        loadAllSprites(grd,*chipset,mode);

        SDL_FreeSurface(chipset);
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
        std::string tipefile = foundSomething? std::string(res,str.end())   : ".png";

        if (!isValidBitmapExtension(tipefile)) 
            throw Exception("graphic bitmap format not valid");

        return std::make_pair(namefile + ".grd",namefile + tipefile);

    }

    void loadAllSprites(const DataGRD& grd, const SDL_Surface& chipset, GraphicMode mode) {
        if (chipset.w / grd._cellwidth <= 0 || chipset.w % grd._cellwidth != 0)
            throw Exception("the grd file doesn't fit with the chipset",__LINE__);

        SDL_Rect     src = {0,0,0,0};
        unsigned int i = 0;
        for (const auto& img : grd._images) {

            src.w = src.x + img.dim.x;
            src.h = src.y + img.dim.y;

            Sprite spt(createResource<SpriteResource>(src,chipset,mode,grd,i++));

            spt.setName(img.name);

            _sprites.push_back(std::move(spt));

            src.x += grd._cellwidth;
            if (src.x + grd._cellwidth > (unsigned int) chipset.w) {
                src.x = 0;
                src.y += grd._cellheight;
                if (src.y + grd._cellheight > (unsigned int) chipset.h)
                    throw Exception("the grd doesn't fit with the chipset",__LINE__);
            }
        }
    }

};

} // flatscene

#endif // FS_SPRITESET_IMPL__
