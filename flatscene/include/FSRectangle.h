#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "SDL.h"
#include "FSPoint.h"

/**
 * FSRect, Rectangle class with absolute coordinates
 *
 * (6,1).__
 *      |  |
 *      |  |
 *      |__.(8,4)
 *
 */
template <class T>
struct FSRect {
    T x,y,w,h;

    FSRect (T x=0, T y=0, T w=0, T h=0):
        x(x), y(y), w(w), h(h) {}

    template <class U>
    FSRect (const U& rc):
        x((T)rc.x), y((T)rc.y), w((T)rc.w), h((T)rc.h) {}

    template <class U1,class U2>
    FSRect (const U1& p1,const U2& p2):
        x((T)p1.x), y((T)p1.y), w((T)p2.x), h((T)p2.y) {}

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
        return SDL_Rect(x,y,w,h);
    }

    inline operator FSPoint() {
        return FSPoint(x,y);
    }

    FSRect<T>& set (T nx,T ny,T nw,T nh) {
        x=nx;
        y=ny;
        w=nw;
        h=nh;
        return *this;
    }

    template <class U>
    inline bool intersect(const U& rc) {
        return (x > rc.w) || (y > rc.h) || (w < rc.x) || (h < rc.y) ?
                false : true;
    }

    inline bool contains(T cx, T cy) {
        return (cx > x) && (cx < w) && (cy > y) && (cy < h);
    }

    template <class U>
    inline bool contains(const U& rc) {
        return contains(rc.x,rc.y) && contains(rc.w,rc.h);
    }
};

typedef FSRect<Sint16> FSRectangle;

/**
 * FSRelRect, Rectangle class with relative coordinates
 *
 * (6,1).__
 *      |  |
 *      |  |
 *      |__.(2,3)
 *
 */

template <class T>
struct FSRelRect : FSRect<T> {
    using FSRect<T>::x;
    using FSRect<T>::w;
    using FSRect<T>::h;
    using FSRect<T>::y;

    template <class U>
    inline bool intersect(const U& rc) {
        return (x > rc.x+rc.w) || (y > rc.y+rc.h) || (x+w < rc.x) || (y+h < rc.y) ?
                false : true;
    }

    inline bool contains(T cx, T cy) {
        return (cx > x) && (cx < x+w) && (cy > y) && (cy < y+h);
    }

    template <class U>
    inline bool contains(const U& rc) {
        return contains(rc.x,rc.y) && contains(rc.w,rc.h);
    }
};

#endif
