#ifndef FS_GRAPHIC_RESOURCE_DESCRIPTOR
#define FS_GRAPHIC_RESOURCE_DESCRIPTOR

#include "Exception.h"
#include "TinyXMLHelper.h"

namespace FlatScene {

	using namespace Util::XML::Tiny;

	struct DocIsNotLoadedException {};

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

            Sprite(DimPoint ndim = DimPoint(),CPoint ncp = CPoint()) : dim(ndim), cp(ncp) {}
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

        DataGRD(const SDL_Surface& chipset)
        : sp_scale(1.0), num_img(1), simple(true), cellwidth(chipset.w), cellheight(chipset.h) {

        	processSimpleSpriteValues();
        	assert(("DataGRD construction by chipset must be wrong", areValuesConsistent()));
        }

    	DataGRD(const std::string& grd_str) {
	        TiXmlDocument doc = getLoadedDocument(grd_str);
	        TiXmlHandle input(doc.FirstChild()); 

	        int i = 0;
	        while (isDefinedInOtherFile(*input.Element())) {
	            if (i++ > 100) throw Exception("100 limit cycles exceeded between grd files",__LINE__);
	            doc = getFromOtherFile(*input.Element());
	            input = TiXmlHandle(doc.FirstChild());
	        }

	        processHeadElement(*input.Element());
	        if (simple) {
	            processSimpleSpriteValues();
	        } else {
		        processGlobalValues(input);
		        processSpriteValues(input);
	    	}

	    	assert(("DataGRD construction by file must be wrong", areValuesConsistent()));
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

	    void processHeadElement(const TiXmlElement& head) {
	        num_img    = numFromAttr<decltype(num_img)   >(head,"sprites");
	        cellwidth  = numFromAttr<decltype(cellwidth) >(head,"cellwidth");
	        cellheight = numFromAttr<decltype(cellheight)>(head,"cellheight");

	        //if (checkAttr(head,"type","split"))
	        //    ; // @TODO return loadChipsetSplit(s_aux,mode);

	        simple = checkAttr(head,"simple","true");
	        if (checkAttr(head,"sp-scale"))
	            sp_scale = numFromAttr<decltype(sp_scale)>(head,"sp-scale", 0.01, 100);
	        else
	            sp_scale = 1.0;
	    }

	    void processGlobalValues(const TiXmlHandle& doc) {
	        if (auto el = doc.FirstChildElement("globalcpoint").Element()) {
	            globalcp.set(
	                numFromAttr<decltype(globalcp.x)>(*el,"x",0,cellwidth),
	                numFromAttr<decltype(globalcp.y)>(*el,"y",0,cellheight)
	            );
	        } else {
	            globalcp.set(0, 0);
	        }

	        fillAreasFromElement(
	            doc.FirstChildElement("globalareas").FirstChildElement("area").ToElement(),
	            globalareas,
	            globalcp,
	            sp_scale,
	            ga_isRel
	        );
	    }

	    void processSimpleSpriteValues() {
			for (decltype(num_img) i = 0; i < num_img ; i++)
                images.push_back(DataGRD::Sprite(
                    DataGRD::DimPoint(cellwidth, cellheight),
                    DataGRD::CPoint  (0,0)
                ));
	    }

	    void processSpriteValues(const TiXmlHandle& doc) {
	        for (auto pImg = doc.FirstChildElement("img").ToElement(); 
	            pImg ; pImg = pImg->NextSiblingElement()) {
	                
	            DataGRD::Sprite spt; const auto& img = *pImg;
	            spt.name = checkAttr(img,"name","",false) ? 
	                valFromAttr<std::string>(img,"name") : "noname" ;
	            spt.dim.set( numFromAttr<decltype(spt.dim.x)>(img,"width",0,cellwidth  ) ,
	                            numFromAttr<decltype(spt.dim.y)>(img,"height",0,cellheight));

	            spt.cp.set( globalcp.x, globalcp.y );
	            if (auto el = img.FirstChildElement("cpoint")) {
	                spt.cp.x += numFromAttr<decltype(spt.cp.x)>(*el,"x",0, spt.dim.x - globalcp.x );
	                spt.cp.y += numFromAttr<decltype(spt.cp.y)>(*el,"y",0, spt.dim.x - globalcp.y );
	            } else if (spt.cp.x > spt.dim.x || spt.cp.y > spt.dim.y)
	                throw Exception("the global cp is not valid due to image sizes",__LINE__);
	                
	            fillAreasFromElement(
	                img.FirstChildElement("area"), 
	                spt.areas, 
	                spt.cp, 
	                sp_scale, 
	                spt.a_isRel
	            );

	            images.push_back(std::move(spt));
	        }
	    }
	        
	    bool areValuesConsistent() {
	        if (num_img != images.size())
	            return false;

	        for (auto& img : images) for (auto& ar : img.areas) for (auto& rc : ar.second)
	            if (rc.x < 0 || rc.y < 0 || rc.w > (decltype(rc.w))img.dim.x || rc.h > (decltype(rc.h))img.dim.y)
	                return false;

	        return true;
	    }
    };

    inline DataGRD loadFileGRD(const std::string& grd_str, const SDL_Surface *const chipset) {
        try {
            return DataGRD(grd_str);
        } catch(DocIsNotLoadedException& e) {
            if (!chipset) 
                throw Exception("grd file invalid and bitmap invalid",__LINE__);

            return DataGRD(*chipset);
        }
    }

} // FlatScene

#endif