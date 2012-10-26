#ifndef FS_GRAPHIC_RESOURCE_DESCRIPTOR
#define FS_GRAPHIC_RESOURCE_DESCRIPTOR

#include <vector>
#include <map>
#include <string>

#include "Types.h"
#include "GraphicTypes.h"

namespace FlatScene {

    struct DocIsNotLoadedException {};

    struct GRDProcess;

    struct GRD {
        typedef std::vector<Rectangle> Area;
        typedef Point2D<unsigned int>  CPoint;
        typedef Point2D<unsigned int>  DimPoint;

        struct Image {
            std::string         name;
            DimPoint            dim;
            CPoint              cp;
            std::map<int,Area>  areas;
            std::map<int,bool>  a_isRel;

            Image(DimPoint ndim = DimPoint(),CPoint ncp = CPoint()) : dim(ndim), cp(ncp) {}
        };

        GRD(unsigned int width, unsigned int height, std::string file);
        GRD(std::string file); /* @throw DocIsNotLoadedException */

        const std::string&          getDescriptorFile() const { return _grd_str;     }
        const std::string&          getGraphicFile   () const { return _gr_file;     }
        double                      getSpecialScale  () const { return _sp_scale;    }
        const std::vector<Image>&   getImages        () const { return _images;      }
        unsigned int                getTeoricQuantity() const { return _num_img;     }
        unsigned int                getCellWidth     () const { return _cellwidth;   }
        unsigned int                getCellHeight    () const { return _cellheight;  }
        bool                        isSimple         () const { return _simple;      }
        const CPoint&               getGlobalCP      () const { return _globalcp;    }
        const std::map<int,Area>&   getGlobalAreas   () const { return _globalareas; }
        const std::map<int,bool>&   getRelativeAreas () const { return _ga_isRel;    }  
    private:

        std::vector<Image>          _images;
        unsigned int                _num_img;
        unsigned int                _cellwidth;
        unsigned int                _cellheight;
        bool                        _simple;
        double                      _sp_scale;
        CPoint                      _globalcp;
        std::map<int,Area>          _globalareas;
        std::map<int,bool>          _ga_isRel;
        std::string                 _grd_str;
        std::string                 _gr_file;

        friend struct GRDProcess;

    };

} // FlatScene

#endif