#include "GraphicResourceDescriptor.h"
#include "Exception.h"
#include "TinyXMLHelper.h"
#include "parserXML.h"

namespace FlatScene {

    using namespace Util::XML::Tiny;

    TiXmlDocument getLoadedDocument(const std::string& str);

    bool isDefinedInOtherFile(const TiXmlElement& head);

    TiXmlDocument getFromOtherFile(const TiXmlElement& head);

    template <typename PointType> 
    void fillAreasFromElement(const TiXmlElement* pArea, std::map<int,DataGRD::Area>& areas, 
        const PointType& cp, double scale, std::map<int,bool>& rel_m );

    void processHeadElement(DataGRD& grd,const TiXmlElement& head);

    void processGlobalValues(DataGRD& grd,const TiXmlHandle& doc);

    void processSimpleSpriteValues(DataGRD& grd);

    void processSpriteValues(DataGRD& grd,const TiXmlHandle& doc);

    bool areValuesConsistent(DataGRD& grd);


    DataGRD::DataGRD(unsigned int width, unsigned int height, const std::string& grd_str)
        : sp_scale(1.0), num_img(1), simple(true), cellwidth(width), cellheight(height), grd_str(grd_str)
    {
        processSimpleSpriteValues(*this);
        assert(("DataGRD construction by size must be wrong", areValuesConsistent(*this)));
    }

    DataGRD::DataGRD(const std::string& grd_str) : grd_str(grd_str) {
        TiXmlDocument doc = getLoadedDocument(grd_str);
        TiXmlHandle input(doc.FirstChild()); 

        int i = 0;
        while (isDefinedInOtherFile(*input.Element())) {
            if (i++ > 100) throw Exception("100 limit cycles exceeded between grd files",__LINE__);
            doc = getFromOtherFile(*input.Element());
            input = TiXmlHandle(doc.FirstChild());
        }

        processHeadElement(*this,*input.Element());
        if (simple) {
            processSimpleSpriteValues(*this);
        } else {
            processGlobalValues(*this,input);
            processSpriteValues(*this,input);
        }

        assert(("DataGRD construction by file must be wrong", areValuesConsistent(*this)));
    }

    TiXmlDocument getLoadedDocument(const std::string& str) {
        TiXmlDocument doc(str.c_str());
        if (!doc.LoadFile())
            throw DocIsNotLoadedException();

        if (!doc.FirstChild() || !doc.FirstChild()->ToElement()) 
            throw Exception("no elements in given document",__LINE__);

        return doc;
    }

    bool isDefinedInOtherFile(const TiXmlElement& head) {
        return checkAttr(head,"defined-in","",false);
    }

    TiXmlDocument getFromOtherFile(const TiXmlElement& head) {
        ensureAttr(head,"defined-in","",false);
        std::string grd_str(head.Attribute("defined-in"));
        return getLoadedDocument(grd_str);
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

    void processSimpleSpriteValues(DataGRD& grd) {
        for (decltype(grd.num_img) i = 0; i < grd.num_img ; i++)
            grd.images.push_back(DataGRD::Sprite(
                DataGRD::DimPoint(grd.cellwidth, grd.cellheight),
                DataGRD::CPoint  (0,0)
            ));
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

    bool areValuesConsistent(DataGRD& grd) {
        if (grd.num_img != grd.images.size())
            return false;

        for (auto& img : grd.images) for (auto& ar : img.areas) for (auto& rc : ar.second)
            if (rc.x < 0 || rc.y < 0 || rc.w > (decltype(rc.w))img.dim.x || rc.h > (decltype(rc.h))img.dim.y)
                return false;

        return true;
    }

};