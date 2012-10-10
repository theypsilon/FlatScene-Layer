#include "CanvasResource.h"
#include "ScreenImpl.h"
#include "Exception.h"
#include "Algorithm.h"
#include "ImageAdapter.h"

namespace FlatScene {

Canvas::Canvas(CanvasResource* res) 
    : ResourceHandler(res)
{}

unsigned int Canvas::getPixel(unsigned int x, unsigned int y) const {
    return getRes().getPixel(x,y);
}

int Canvas::getWidth  () const { return getRes().getW(); }
int Canvas::getHeight () const { return getRes().getH(); }

void Canvas::put ( const FloatPoint& ptDst, unsigned char flags) const {
    getRes().put(ptDst,flags);
}

void Canvas::put ( const Point& ptDst, unsigned char flags) const {
    getRes().put(ptDst,flags);
}

void Canvas::rotate(Float angle, Float x, Float y, Float z) const {
    getRes().initCallbackList.push_back([=](){
        Screen::I().rotate(angle,x,y,z);
    });
}
void Canvas::translate(Float x, Float y, Float z) const {
    getRes().initCallbackList.push_back([=](){
        Screen::I().translate(x,y,z);
    });
}

void Canvas::scale(Float x, Float y, Float z) const {
    getRes().initCallbackList.push_back([=](){
        Screen::I().scale(x,y,z);
    });
}

void Canvas::color(Float red, Float green, Float blue, Float alpha) const {

    if (red > 1.0) red = 1.0;
    if (green > 1.0) green = 1.0;
    if (blue > 1.0) blue = 1.0;
    if (alpha > 1.0) alpha = 1.0;

    getRes().initCallbackList.push_back([=](){
        Screen::I().color(red,green,blue,alpha);
    });

    red   = Screen::I()._impl->red  ; //2.0 - red;
    green = Screen::I()._impl->green; //2.0 - green;
    blue  = Screen::I()._impl->blue ; //2.0 - blue;
    alpha = Screen::I()._impl->alpha; //2.0 - alpha;

    getRes().endCallbackList.push_back([=](){
        Screen::I().color(red,green,blue,alpha);
    });
}

void Canvas::color(const Color& col, Float alpha) const {
    color(  ((Float) col.getR() ) / 255.0,
            ((Float) col.getG() ) / 255.0,
            ((Float) col.getB() ) / 255.0,
            alpha );
}

} // flatscene
