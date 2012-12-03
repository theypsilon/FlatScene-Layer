#include "CanvasResourceRendering.h"
#include "ScreenImpl.h"
#include "Exception.h"
#include "Algorithm.h"

namespace FlatScene {

unsigned int Canvas::getPixel(unsigned int x, unsigned int y) const {
    return _res->getPixel(x,y);
}

int Canvas::getWidth  () const { return _res->getW(); }
int Canvas::getHeight () const { return _res->getH(); }

void Canvas::put (Float x, Float y, unsigned char flags) const {
    _res->put(x,y,flags);
}

void Canvas::rotate(Float angle, Float x, Float y, Float z) const {
    _res->rotate(angle,x,y,z);
}
void Canvas::translate(Float x, Float y, Float z) const {
    _res->translate(x,y,z);
}

void Canvas::scale(Float x, Float y, Float z) const {
    _res->scale(x,y,z);
}

void Canvas::color(Float red, Float green, Float blue, Float alpha) const {
    _res->color(red,green,blue,alpha);
}

void Canvas::color(const Color& col, Float alpha) const {
    color(  ((Float) col.getR() ) / 255.0,
            ((Float) col.getG() ) / 255.0,
            ((Float) col.getB() ) / 255.0,
            alpha );
}


unsigned int SoftwareCanvas::getPixel(unsigned int x, unsigned int y) const {
    return _res->getPixel(x,y);
}

int SoftwareCanvas::getWidth  () const { return _res->getW(); }
int SoftwareCanvas::getHeight () const { return _res->getH(); }


} // flatscene
