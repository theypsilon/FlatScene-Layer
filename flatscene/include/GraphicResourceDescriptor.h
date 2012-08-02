#ifndef FS_GRAPHIC_RESOURCE_DESCRIPTOR
#define FS_GRAPHIC_RESOURCE_DESCRIPTOR

#include <vector>
#include <map>
#include <string>

#include "Types.h"

namespace FlatScene {

    struct DocIsNotLoadedException {};

    struct GRDProcess;

    struct GRD {
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

        GRD(unsigned int width, unsigned int height, std::string file);
        GRD(std::string file); /* @throw DocIsNotLoadedException */

        const std::string&  getDescriptorFile() const { return _grd_str; }
        const std::string&  getGraphicFile() const { return _gr_file; }
        double              getSpecialScale() const { return _sp_scale; }

    private:

        std::vector<Sprite> _images;
        unsigned int        _num_img;
        unsigned int        _cellwidth;
        unsigned int        _cellheight;
        bool                _simple;
        double              _sp_scale;
        CPoint              _globalcp;
        std::map<int,Area>  _globalareas;
        std::map<int,bool>  _ga_isRel;
        std::string         _grd_str;
        std::string         _gr_file;

        friend class SpritesetResource;
        friend struct GRDProcess;

    };

} // FlatScene

#endif