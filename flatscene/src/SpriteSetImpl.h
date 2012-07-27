#ifndef FS_SPRITESET_IMPL__
#define FS_SPRITESET_IMPL__

#include "SpriteSet.h"
#include "Exception.h"
#include "parserXML.h"
#include "Library.h"
#include "debugfuncs.h"

#include <algorithm>
#include <iostream>

#include "TinyXMLHelper.h"
#include "CanvasFactory.h"
#include "SpriteImpl.h"

#include "GraphicResourceDescriptor.h"

namespace FlatScene {

using namespace Util::XML::Tiny;

struct Spriteset::SpritesetImpl {

    typedef std::vector<Sprite> SpriteCollection;

    SpritesetImpl(const std::string& c, unsigned char mode) 
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

    int size () const {
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
        return bitmap == ".png" || bitmap == ".bmp" || bitmap == ".jpg";
    }

    std::pair<std::string, std::string> getNameFile(const std::string& str) {

        std::string tipefile;
        std::string namefile;

        auto res = str.end();
        for(auto it=str.end(); it != str.begin() ; --it)
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

    void loadAllSprites(DataGRD& grd, const SDL_Surface& chipset, unsigned char mode) {
        if (chipset.w / grd.cellwidth <= 0 || chipset.w % grd.cellwidth != 0)
            throw Exception("the grd file doesn't fit with the chipset",__LINE__);

        if (grd.simple) {
            DataGRD::Sprite spt;
            spt.dim.set( grd.cellwidth, grd.cellheight );
            spt.cp.set(0,0);

            for (decltype(grd.num_img) i = 0; i < grd.num_img ; i++)
                grd.images.push_back(std::move(spt));
        }

        SDL_Rect src = {0,0,0,0};
        for (const auto& img : grd.images) {

            src.w = src.x + img.dim.x;
            src.h = src.y + img.dim.y;

            Sprite spt = loadSprite(src,chipset,mode,grd.sp_scale);

            spt.name = std::move(img.name);

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

    Sprite loadSprite(const SDL_Rect& src, const SDL_Surface& chipset, unsigned char mode, double sp_scale) {
        auto surf = SDL_CreateRGBSurface(chipset.flags | SDL_SRCALPHA,
                                         src.w - src.x, src.h - src.y,
                                         chipset.format->BitsPerPixel,
                                         chipset.format->Rmask, chipset.format->Gmask,
                                         chipset.format->Bmask, chipset.format->Amask);

        SDL_SetColorKey(surf,SDL_SRCCOLORKEY, chipset.format->colorkey);
        blitcopy(chipset,const_cast<SDL_Rect*>(&src),surf,nullptr);

        if (sp_scale != 1.0 && mode != ONLY_SDL_SURFACE) {
            if (auto temp = Canvas::scaleSurface(surf,(int)sp_scale)) {
                SDL_FreeSurface(surf);
                surf=temp;
            }
            // Reasignamos los formatos.
            SDL_SetColorKey(surf,SDL_SRCCOLORKEY,chipset.format->colorkey);
        }

        Sprite spt = Canvas::createCanvas<Sprite>(surf,mode);

        //Sprite spt(Canvas::toSCanvas(surf,mode),img.cp);

        return spt;
    }
};

} // flatscene

#endif // FS_SPRITESET_IMPL__
