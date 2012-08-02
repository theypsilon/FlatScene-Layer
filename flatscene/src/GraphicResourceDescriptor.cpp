#include "GraphicResourceDescriptor.h"
#include "Exception.h"
#include "TinyXMLHelper.h"
#include "parserXML.h"

namespace FlatScene {

    using namespace Util::XML::Tiny;

    struct GRDProcess {

        static TiXmlDocument getLoadedDocument(const std::string& str);

        static bool isDefinedInOtherFile(const TiXmlElement& head);

        static TiXmlDocument getFromOtherFile(const TiXmlElement& head);

        template <typename PointType> 
        static void fillAreasFromElement(const TiXmlElement* pArea, std::map<int,DataGRD::Area>& areas, 
            const PointType& cp, double scale, std::map<int,bool>& rel_m );

        static void processHeadElement(DataGRD& grd,const TiXmlElement& head);

        static void processGlobalValues(DataGRD& grd,const TiXmlHandle& doc);

        static void processSimpleSpriteValues(DataGRD& grd);

        static void processSpriteValues(DataGRD& grd,const TiXmlHandle& doc);

        static bool areValuesConsistent(DataGRD& grd);

    };


    DataGRD::DataGRD(unsigned int width, unsigned int height, std::string file)
        : _sp_scale(1.0), _num_img(1), _simple(true), _cellwidth(width), _cellheight(height), _grd_str(std::move(file))
    {
        typedef GRDProcess p;
        p::processSimpleSpriteValues(*this);
        assert(("DataGRD construction by size must be wrong", p::areValuesConsistent(*this)));
    }

    DataGRD::DataGRD(std::string file) : _grd_str(std::move(file)) {
        typedef GRDProcess p;
        TiXmlDocument doc = p::getLoadedDocument(_grd_str);
        TiXmlHandle input(doc.FirstChild()); 

        int i = 0;
        while (p::isDefinedInOtherFile(*input.Element())) {
            if (i++ > 100) throw Exception("100 limit cycles exceeded between grd files",__LINE__);
            doc = p::getFromOtherFile(*input.Element());
            input = TiXmlHandle(doc.FirstChild());
        }

        p::processHeadElement(*this,*input.Element());
        if (_simple) {
            p::processSimpleSpriteValues(*this);
        } else {
            p::processGlobalValues(*this,input);
            p::processSpriteValues(*this,input);
        }

        assert(("DataGRD construction by file must be wrong", p::areValuesConsistent(*this)));
    }

    TiXmlDocument GRDProcess::getLoadedDocument(const std::string& str) {
        TiXmlDocument doc(str.c_str());
        if (!doc.LoadFile())
            throw DocIsNotLoadedException();

        if (!doc.FirstChild() || !doc.FirstChild()->ToElement()) 
            throw Exception("no elements in given document",__LINE__);

        return doc;
    }

    bool GRDProcess::isDefinedInOtherFile(const TiXmlElement& head) {
        return checkAttr(head,"defined-in","",false);
    }

    TiXmlDocument GRDProcess::getFromOtherFile(const TiXmlElement& head) {
        ensureAttr(head,"defined-in","",false);
        std::string grd_str(head.Attribute("defined-in"));
        return getLoadedDocument(grd_str);
    }

    template <typename PointType> 
    void GRDProcess::fillAreasFromElement(const TiXmlElement* pArea, std::map<int,DataGRD::Area>& areas, 
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

    void GRDProcess::processHeadElement(DataGRD& grd, const TiXmlElement& head) {
        grd._num_img    = numFromAttr<decltype(grd._num_img)   >(head,"sprites");
        grd._cellwidth  = numFromAttr<decltype(grd._cellwidth) >(head,"cellwidth");
        grd._cellheight = numFromAttr<decltype(grd._cellheight)>(head,"cellheight");

        //if (checkAttr(head,"type","split"))
        //    ; // @TODO return loadChipsetSplit(s_aux,mode);

        grd._simple = checkAttr(head,"simple","true");
        if (checkAttr(head,"sp-scale"))
            grd._sp_scale = numFromAttr<decltype(grd._sp_scale)>(head,"sp-scale", 0.01, 100);
        else
            grd._sp_scale = 1.0;
    }

    void GRDProcess::processGlobalValues(DataGRD& grd, const TiXmlHandle& doc) {
        if (auto el = doc.FirstChildElement("globalcpoint").Element()) {
            grd._globalcp.set(
                numFromAttr<decltype(grd._globalcp.x)>(*el,"x",0,grd._cellwidth),
                numFromAttr<decltype(grd._globalcp.y)>(*el,"y",0,grd._cellheight)
            );
        } else {
            grd._globalcp.set(0, 0);
        }

        fillAreasFromElement(
            doc.FirstChildElement("globalareas").FirstChildElement("area").ToElement(),
            grd._globalareas,
            grd._globalcp,
            grd._sp_scale,
            grd._ga_isRel
        );
    }

    void GRDProcess::processSimpleSpriteValues(DataGRD& grd) {
        for (decltype(grd._num_img) i = 0; i < grd._num_img ; i++)
            grd._images.push_back(DataGRD::Sprite(
                DataGRD::DimPoint(grd._cellwidth, grd._cellheight),
                DataGRD::CPoint  (0,0)
            ));
    }

    void GRDProcess::processSpriteValues(DataGRD& grd, const TiXmlHandle& doc) {
        for (auto pImg = doc.FirstChildElement("img").ToElement(); 
            pImg ; pImg = pImg->NextSiblingElement()) {

                DataGRD::Sprite spt; const auto& img = *pImg;
                spt.name = checkAttr(img,"name","",false) ? 
                    valFromAttr<std::string>(img,"name") : "noname" ;
                spt.dim.set( numFromAttr<decltype(spt.dim.x)>(img,"width",0,grd._cellwidth  ) ,
                    numFromAttr<decltype(spt.dim.y)>(img,"height",0,grd._cellheight));

                spt.cp.set( grd._globalcp.x, grd._globalcp.y );
                if (auto el = img.FirstChildElement("cpoint")) {
                    spt.cp.x += numFromAttr<decltype(spt.cp.x)>(*el,"x",0, spt.dim.x - grd._globalcp.x );
                    spt.cp.y += numFromAttr<decltype(spt.cp.y)>(*el,"y",0, spt.dim.x - grd._globalcp.y );
                } else if (spt.cp.x > spt.dim.x || spt.cp.y > spt.dim.y)
                    throw Exception("the global cp is not valid due to image sizes",__LINE__);

                fillAreasFromElement(
                    img.FirstChildElement("area"), 
                    spt.areas, 
                    spt.cp, 
                    grd._sp_scale, 
                    spt.a_isRel
                );

                grd._images.push_back(std::move(spt));
        }
    }

    bool GRDProcess::areValuesConsistent(DataGRD& grd) {
        if (grd._num_img != grd._images.size())
            return false;

        for (auto& img : grd._images) for (auto& ar : img.areas) for (auto& rc : ar.second)
            if (rc.x < 0 || rc.y < 0 || rc.w > (decltype(rc.w))img.dim.x || rc.h > (decltype(rc.h))img.dim.y)
                return false;

        return true;
    }

};