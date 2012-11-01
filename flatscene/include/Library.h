//include guards
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//include message handler(base class)

namespace FlatScene {

    int startLibrary(bool xmlconfig);

    int startLibrary( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;


} // flatscene

#endif //#ifndef __APPLICATION_H__
