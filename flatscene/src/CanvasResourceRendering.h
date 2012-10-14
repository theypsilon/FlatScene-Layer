#ifndef FS_CANVAS_RENDERING__
#define FS_CANVAS_RENDERING__

#include "CanvasResource.h"
#include "ScreenImpl.h"

namespace FlatScene {

    inline void CanvasResource::rotate(Float angle, Float x, Float y, Float z) const {
        _initCallbackList.push_back([=](){
            Screen::I().rotate(angle,x,y,z);
        });
    }
    inline void CanvasResource::translate(Float x, Float y, Float z) const {
        _initCallbackList.push_back([=](){
            Screen::I().translate(x,y,z);
        });
    }

    inline void CanvasResource::scale(Float x, Float y, Float z) const {
        _initCallbackList.push_back([=](){
            Screen::I().scale(x,y,z);
        });
    }

    inline void CanvasResource::color(Float red, Float green, Float blue, Float alpha) const {

        if (red > 1.0) red = 1.0;
        if (green > 1.0) green = 1.0;
        if (blue > 1.0) blue = 1.0;
        if (alpha > 1.0) alpha = 1.0;

        _initCallbackList.push_back([=](){
            Screen::I().color(red,green,blue,alpha);
        });

        red   = Screen::I()._impl->red  ; //2.0 - red;
        green = Screen::I()._impl->green; //2.0 - green;
        blue  = Screen::I()._impl->blue ; //2.0 - blue;
        alpha = Screen::I()._impl->alpha; //2.0 - alpha;

        _endCallbackList.push_back([=](){
            Screen::I().color(red,green,blue,alpha);
        });
    }

} // flatscene

#endif