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
#include "CanvasFactory.h"
#include "SpriteResource.h"
#include "RefCountMemoryPolicyImpl.h"

#include "GraphicResourceDescriptor.h"

namespace FlatScene {

using namespace Util::XML::Tiny;

class SpritesetResource {
public:

    typedef std::vector<Sprite> SpriteCollection;

    SpritesetResource(const std::string& c, unsigned char mode) 
        : _mode(mode), _name(c) {
            loadChipset(c,mode);
    }

    Uint8 getMode() {
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
    unsigned char    _mode;

    void loadChipset(const std::string& c,unsigned char mode=ONLY_TEXTURE,std::string* cPrev=nullptr) {
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

        std::string tipefile;
        std::string namefile;

        auto res = str.end();
        for(auto it=str.begin(); it != str.end() ; ++it)
            if (*it == '.')
                res = it;

        if (res != str.end()) {
            namefile += std::string(str.begin(),res);
            tipefile += std::string(res,str.end());
        } else {
            namefile += str;
            tipefile += ".png";
        }

        if (!isValidBitmapExtension(tipefile)) 
            throw Exception("graphic bitmap format not valid");

        return std::make_pair(namefile + ".grd",namefile + tipefile);

    }

    void loadAllSprites(const DataGRD& grd, const SDL_Surface& chipset, unsigned char mode) {
        if (chipset.w / grd.cellwidth <= 0 || chipset.w % grd.cellwidth != 0)
            throw Exception("the grd file doesn't fit with the chipset",__LINE__);

        SDL_Rect src = {0,0,0,0};
        for (const auto& img : grd.images) {

            src.w = src.x + img.dim.x;
            src.h = src.y + img.dim.y;

            Sprite spt = createCanvas<Sprite>(src,chipset,mode,grd.sp_scale);

            //spt.setName(std::move(img.name));

            _sprites.push_back(std::move(spt));

            src.x += grd.cellwidth;
            if (src.x + grd.cellwidth > (unsigned int) chipset.w) {
                src.x = 0;
                src.y += grd.cellheight;
                if (src.y + grd.cellheight > (unsigned int) chipset.h)
                    throw Exception("the grd doesn't fit with the chipset",__LINE__);
            }
        }
    }

};

} // flatscene

#endif // FS_SPRITESET_IMPL__
