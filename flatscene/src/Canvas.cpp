#include "CanvasResource.h"
#include "ScreenImpl.h"
#include "Renders.h"
#include "Exception.h"
#include "Algorithm.h"
#include "ImageAdapter.h"

namespace FlatScene {

Canvas::Canvas(CanvasResource* res) 
    : ResourceHandler(res)
{}

unsigned int Canvas::getPixel(unsigned int x, unsigned int y) const {
    return FlatScene::getPixel(x,y,getRes().raw);
}

int Canvas::getWidth  () const { return getRes().w2; }
int Canvas::getHeight () const { return getRes().h2; }

template <typename PointType, typename GraphicMaterial>
inline void putCanvas ( const PointType& ptDst, unsigned char flags, 
                        const CanvasResource& impl, GraphicMaterial& gm ) {

    Screen::I().pushMatrix();
    Screen::I().translate(ptDst.x,ptDst.y,0);

    // USER DEFINED EFFECTS IN

    call_to_all(impl.initCallbackList);
    impl.initCallbackList.clear();

    gm.push_back(
        new SRenderCanvas(impl,flags)
    );

    // USER DEFINED EFFECTS OUT

    call_to_all(impl.endCallbackList);
    impl.endCallbackList.clear();

    Screen::I().popMatrix();
}

void Canvas::put ( const FloatPoint& ptDst, unsigned char flags) const {
    putCanvas( ptDst, flags, getRes(), Screen::I()._impl->graphicMaterial );
}

void Canvas::put ( const Point& ptDst, unsigned char flags) const {
    putCanvas( ptDst, flags, getRes(), Screen::I()._impl->graphicMaterial );
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
