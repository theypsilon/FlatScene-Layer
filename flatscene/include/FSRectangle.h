#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "SDL.h"
#include "FSPoint.h"

template <class T>
struct FSRect {
    T x,y,w,h;

    FSRect (T x=0, T y=0, T w=0, T h=0):
        x{x}, y{y}, w{w}, h{h} {}

    FSRect ( SDL_Rect& rc ):
        x{(T)rc.x}, y{(T)rc.y}, w{(T)rc.w}, h{(T)rc.h} {}

    FSRect ( SDL_Rect* rc ):
        x{(T)rc->x}, y{(T)rc->y}, w{(T)rc->w}, h{(T)rc->h} {}

    template <class U>
    FSRect ( FSRect<U>& rc ):
        x{(T)rc.x}, y{(T)rc.y}, w{(T)rc.w}, h{(T)rc.h} {}

    inline T& X() { return x; }
    inline T& Y() { return y; }
    inline T& W() { return w; }
    inline T& H() { return h; }

    inline T getX() const { return x; }
    inline T getY() const { return y; }
    inline T getW() const { return w; }
    inline T getH() const { return h; }

    void setX(T nx) { x = nx; }
    void setY(T ny) { y = ny; }
    void setW(T nw) { w = nw; }
    void setH(T nh) { h = nh; }

    inline operator SDL_Rect() {
        return SDL_Rect{x,y,w,h};
    }

    inline operator FSPoint() {
        return FSPoint{x,y};
    }

    FSRect<T>& set (T nx,T ny,T nw,T nh) {
        x=nx;
        y=ny;
        w=nw;
        h=nh;
        return *this;
    }

    //intersect with another rectangle
    bool Intersect ( FSRect<T>& rc ) {
        return (
        (x >= rc.x && x <= rc.w && y >= rc.y && y <= rc.h) ||
        (w >= rc.x && w <= rc.w && y >= rc.y && y <= rc.h) ||
        (x >= rc.x && x <= rc.w && h >= rc.y && h <= rc.h) ||
        (w >= rc.x && w <= rc.w && h >= rc.y && h <= rc.h) );
    }
/*
    //check if a point is within the rectangle
    bool Contains ( T x , T y ) ;
    bool Contains ( FSPoint& pt ) ;

    //clip or wrap points
    FSPoint Clip ( FSPoint pt ) ;
    FSPoint Wrap ( FSPoint pt ) ;*/
};

typedef FSRect<Sint16> FSRectangle;

#endif
