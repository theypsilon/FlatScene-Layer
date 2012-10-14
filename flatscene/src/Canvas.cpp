#include "CanvasResourceRendering.h"
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

void Canvas::put (Float x, Float y, unsigned char flags) const {
    getRes().put(x,y,flags);
}

void Canvas::rotate(Float angle, Float x, Float y, Float z) const {
    getRes().rotate(angle,x,y,z);
}
void Canvas::translate(Float x, Float y, Float z) const {
    getRes().translate(x,y,z);
}

void Canvas::scale(Float x, Float y, Float z) const {
    getRes().scale(x,y,z);
}

void Canvas::color(Float red, Float green, Float blue, Float alpha) const {
    getRes().color(red,green,blue,alpha);
}

void Canvas::color(const Color& col, Float alpha) const {
    color(  ((Float) col.getR() ) / 255.0,
            ((Float) col.getG() ) / 255.0,
            ((Float) col.getB() ) / 255.0,
            alpha );
}


SoftwareCanvas::SoftwareCanvas(CanvasResource* res) 
    : ResourceHandler(res)
{}

unsigned int SoftwareCanvas::getPixel(unsigned int x, unsigned int y) const {
    return getRes().getPixel(x,y);
}

int SoftwareCanvas::getWidth  () const { return getRes().getW(); }
int SoftwareCanvas::getHeight () const { return getRes().getH(); }


} // flatscene
