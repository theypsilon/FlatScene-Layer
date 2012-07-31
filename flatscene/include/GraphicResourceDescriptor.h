#ifndef FS_GRAPHIC_RESOURCE_DESCRIPTOR
#define FS_GRAPHIC_RESOURCE_DESCRIPTOR

#include <vector>
#include <map>
#include <string>

#include "Types.h"

namespace FlatScene {

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
        std::string         grd_str;

        DataGRD(unsigned int width, unsigned int height, const std::string& grd_str);
        DataGRD(const std::string& grd_str); /* @throw DocIsNotLoadedException */

    };

} // FlatScene

#endif