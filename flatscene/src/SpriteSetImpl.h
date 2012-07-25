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

namespace FlatScene {

using namespace Util::XML::Tiny;

struct Spriteset::SpritesetImpl {

    typedef std::vector<Sprite> SpriteCollection;

    SpritesetImpl(std::string c, Uint8 mode) 
        : mode(mode), name(c) {
            loadChipset(c,mode);
    }

    SpriteCollection m_vecSprites ;
    std::string      name;
    Uint8            mode;

    Uint8 getMode() {
        return mode;
    }

    const std::string& getName() const {
        return name;
    }

    bool setName(std::string& name) {
        if (this->name=="") {
            this->name=name;
            return true;
        }
        return false;
    }

    void add ( Sprite pspt ) {
        //m_vecSprites.push_back ( pspt ) ;
    }

    const Sprite* get ( unsigned int n ) const {
        if ( n < m_vecSprites.size()) {
            return &m_vecSprites.at(n);
        } else {
            return nullptr;
        }
    }
    int size ( ) const {
        return ( m_vecSprites.size ( ) ) ;
    }

    void loadChipset(std::string& c,Uint8 mode=ONLY_TEXTURE,std::string* cPrev=NULL) {
        std::string grd_str, chipset_str;

        getNameFile(c,grd_str,chipset_str);

        auto chipset = IMG_Load(chipset_str.c_str());
        if (!chipset)
            throw Exception("chipset couldn't load");

        auto grd = loadFileGRD(grd_str,chipset);

        loadAllSprites(grd,*chipset,mode);

        SDL_FreeSurface(chipset);
    }

    bool isValidBitmapExtension(const std::string& bitmap) {
        return bitmap == ".png" || bitmap == ".bmp" || bitmap == ".jpg";
    }

    void getNameFile(const std::string& str, std::string& grd, std::string& bitmap) {

        std::string tipefile;
        std::string namefile;

        auto res = str.end();
        for(auto it=str.begin(), et=str.end(); it != et ; ++it)
            if (*it == '.')
                res = it;

        if (res != str.end()) {
            namefile = std::string(str.begin(),res);
            tipefile = std::string(res,str.end());
        } else {
            namefile = str;
            tipefile = ".png";
        }

        if (!isValidBitmapExtension(tipefile)) 
            throw Exception("graphic bitmap format not valid");

        grd = namefile + ".grd";
        bitmap = namefile + tipefile;

    }

    struct DataGRD {
        typedef std::vector<Rectangle> Area;
        typedef Point2D<unsigned int>  CPoint;
        typedef Point2D<unsigned int>  DimPoint;
        struct Sprite {
            std::string         name;
            DimPoint            dim;
            CPoint              cp;
            std::map<int,Area>  areas;
            std::map<int,bool>  a_isRel;
        };

        std::vector<Sprite> images;
        unsigned int        num_img;
        unsigned int        cellwidth;
        unsigned int        cellheight;
        bool                simple;
        double              sp_scale;
        CPoint              globalcp;
        std::map<int,Area>  globalareas;
        std::map<int,bool>  ga_isRel;
    };

    struct DocIsNotLoadedException {};

    DataGRD loadFileGRD(const std::string& grd_str, const SDL_Surface *const chipset = nullptr) {
        try {
            return fillGRDFromDocument(grd_str);
        } catch(DocIsNotLoadedException& e) {
            if (!chipset) 
                throw Exception("grd file invalid and bitmap invalid",__LINE__);

            return fillGRDFromChipset(*chipset);
        }
    }

    TiXmlHandle getHandlerFromDocument(const std::string& str) {
        TiXmlDocument doc(str.c_str());
        if (!doc.LoadFile())
            throw DocIsNotLoadedException();

        TiXmlHandle input(doc.FirstChild()); 
        if (!input.ToElement()) 
            throw Exception("no elements in grd file",__LINE__);

        return input;
    }

    DataGRD fillGRDFromDocument(const std::string& grd_str) {
        TiXmlDocument doc(grd_str.c_str());
        if (!doc.LoadFile())
            throw DocIsNotLoadedException();

        TiXmlHandle input = getHandlerFromDocument(grd_str);

        int i = 0;
        while (isDefinedInOtherFile(*input.Element())) {
            if (i++ > 100) throw Exception("infinite cycle between grd files",__LINE__);
            input = getFromOtherFile(*input.Element());
        }

        auto& head = *input.Element();
        DataGRD grd;

        processHeadElement(grd,head);
        if (grd.simple) 
            return grd;

        processGlobalValues(grd,input);
        processSpriteValues(grd,input);

        ensureConsistentValues(grd);
        return grd;
    }

    DataGRD fillGRDFromChipset(const SDL_Surface& chipset) {
        DataGRD grd;
        grd.sp_scale = 1.0;
        grd.num_img = 1;
        grd.simple = true;
        grd.cellwidth  = chipset.w;
        grd.cellheight = chipset.h;
        return grd;
    }

    template <typename PointType> 
    void fillAreasFromElement(const TiXmlElement* pArea, std::map<int,DataGRD::Area>& areas, 
                                const PointType& cp, double scale, std::map<int,bool>& rel_m ) {
        for ( ; pArea ; pArea = pArea->NextSiblingElement()) {
            int id = numFromAttr(*pArea,"id",0);
            std::remove_reference<decltype(areas.at(0))>::type area;

            bool rel = checkAttr(*pArea,"relative","true");
            for (auto pRect = pArea->FirstChildElement("rectangle"); 
                    pRect; pRect = pRect->NextSiblingElement() )
                    area.push_back
                      (((int)(scale * (intFromAttr(*pRect,"x1") + (rel? cp.x : 0)) ) ,
                        (int)(scale * (intFromAttr(*pRect,"y1") + (rel? cp.y : 0)) ) ,
                        (int)(scale * (intFromAttr(*pRect,"x2") + (rel? cp.x : 0)) ) ,
                        (int)(scale * (intFromAttr(*pRect,"y2") + (rel? cp.y : 0)))));

            areas[id] = std::move(area);
            rel_m[id] = rel;
        }
    }

    bool isDefinedInOtherFile(const TiXmlElement& head) {
        return checkAttr(head,"defined-in","",false);
    }

    TiXmlHandle getFromOtherFile(const TiXmlElement& head) {
        ensureAttr(head,"defined-in","",false);
        std::string grd_str(head.Attribute("defined-in"));
        return getHandlerFromDocument(grd_str);
    }

    void processHeadElement(DataGRD& grd, const TiXmlElement& head) {
        grd.num_img    = numFromAttr<decltype(grd.num_img)   >(head,"sprites");
        grd.cellwidth  = numFromAttr<decltype(grd.cellwidth) >(head,"cellwidth");
        grd.cellheight = numFromAttr<decltype(grd.cellheight)>(head,"cellheight");

        //if (checkAttr(head,"type","split"))
        //    ; // @TODO return loadChipsetSplit(s_aux,mode);

        grd.simple = checkAttr(head,"simple","true");
        if (checkAttr(head,"sp-scale"))
            grd.sp_scale = numFromAttr<decltype(grd.sp_scale)>(head,"sp-scale", 0.01, 100);
        else
            grd.sp_scale = 1.0;
    }

    void processGlobalValues(DataGRD& grd, const TiXmlHandle& doc) {
        if (auto el = doc.FirstChildElement("globalcpoint").Element()) {
            grd.globalcp.set(
                numFromAttr<decltype(grd.globalcp.x)>(*el,"x",0,grd.cellwidth),
                numFromAttr<decltype(grd.globalcp.y)>(*el,"y",0,grd.cellheight)
            );
        } else {
            grd.globalcp.set(0, 0);
        }

        fillAreasFromElement(
            doc.FirstChildElement("globalareas").FirstChildElement("area").ToElement(),
            grd.globalareas,
            grd.globalcp,
            grd.sp_scale,
            grd.ga_isRel
        );
    }

    void processSpriteValues(DataGRD& grd, const TiXmlHandle& doc) {
        for (auto pImg = doc.FirstChildElement("img").ToElement(); 
            pImg ; pImg = pImg->NextSiblingElement()) {
                
            DataGRD::Sprite spt; const auto& img = *pImg;
            spt.name = checkAttr(img,"name","",false) ? 
                valFromAttr<std::string>(img,"name") : "noname" ;
            spt.dim.set( numFromAttr<decltype(spt.dim.x)>(img,"width",0,grd.cellwidth  ) ,
                            numFromAttr<decltype(spt.dim.y)>(img,"height",0,grd.cellheight));

            spt.cp.set( grd.globalcp.x, grd.globalcp.y );
            if (auto el = img.FirstChildElement("cpoint")) {
                spt.cp.x += numFromAttr<decltype(spt.cp.x)>(*el,"x",0, spt.dim.x - grd.globalcp.x );
                spt.cp.y += numFromAttr<decltype(spt.cp.y)>(*el,"y",0, spt.dim.x - grd.globalcp.y );
            } else if (spt.cp.x > spt.dim.x || spt.cp.y > spt.dim.y)
                throw Exception("the global cp is not valid due to image sizes",__LINE__);
                
            fillAreasFromElement(
                img.FirstChildElement("area"), 
                spt.areas, 
                spt.cp, 
                grd.sp_scale, 
                spt.a_isRel
            );

            grd.images.push_back(std::move(spt));
        }
    }
        
    void ensureConsistentValues(const DataGRD& grd) {
        if (grd.simple) return;

        if (grd.num_img != grd.images.size())
            throw Exception("image count doesn't match in grd file",__LINE__);

        for (auto& img : grd.images) for (auto& ar : img.areas) for (auto& rc : ar.second)
            if (rc.x < 0 || rc.y < 0 || rc.w > (decltype(rc.w))img.dim.x || rc.h > (decltype(rc.h))img.dim.y)
                throw Exception("areas not defined within the sprite domain",__LINE__);
    }

    void loadAllSprites(DataGRD& grd, const SDL_Surface& chipset, unsigned char mode) {
        unsigned int columns = chipset.w / grd.cellwidth;
        if (columns <= 0 || chipset.w % grd.cellwidth != 0)
            throw Exception("the grd doesn't fit with the chipset",__LINE__);

        if (grd.simple) {
            DataGRD::Sprite spt;
            spt.dim.set( grd.cellwidth, grd.cellheight );
            spt.cp.set(0,0);

            for (decltype(grd.num_img) i = 0; i < grd.num_img ; i++)
                grd.images.push_back(spt);
        }

        SDL_Rect src = {0,0,0,0};
        for (auto& img : grd.images) {

            src.w = src.x + img.dim.x;
            src.h = src.y + img.dim.y;

            Sprite spt = loadSprite(src,chipset,mode,grd.sp_scale);

            spt.name = std::move(img.name);

            m_vecSprites.push_back(std::move(spt));

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
