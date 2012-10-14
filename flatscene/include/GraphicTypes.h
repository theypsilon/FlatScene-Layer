#ifndef FS_GRAPHIC_TYPES_H
#define FS_GRAPHIC_TYPES_H

namespace FlatScene {

    enum GraphicMode {  // Changes the memory used for containing the image data
        ONLY_CPU,       // -The image can not be rendered
        ONLY_GPU,       // -The pixel data can't be readed, getPixel now throws
        BOTH            // -No drawbacks in features, but it uses double memory
    };

    enum GraphicFilter {
        NEAREST,
        LINEAR
    };

} // FlatScene

#endif