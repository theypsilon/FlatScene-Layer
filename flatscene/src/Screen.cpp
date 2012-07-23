#include "ScreenImpl.h"
#include "Library.h"
#include "Engine.h"
#include "glAdapter.h"

namespace FlatScene {

Screen::Screen() : _impl(new ScreenImpl) {
    _impl->m_SDL_Surface = nullptr;

    _impl->m_FullScreen  = false;
    _impl->m_Doublebuff  = true;
    _impl->rendering     = false;
    _impl->m_maxZ        = 400.0f;

    _impl->trp           = TRP_PERSPECTIVE;

    _impl->m_Bpp         = 0;
    _impl->m_Width       = 0;
    _impl->m_Height      = 0;

    _impl->alpha         = 1.0f;
    _impl->red           = 1.0f;
    _impl->green         = 1.0f;
    _impl->blue          = 1.0f;
}

Screen::~Screen() {
    delete _impl;
}

int Screen::start(int width, int height, int bpp, bool fullscreen, bool doublebuff)
{ 

    if (_impl->m_SDL_Surface) throw Exception("Video ya inicializado, orden imposible 'start'\n");

    if (SDL_InitSubSystem(SDL_INIT_VIDEO)==-1) throw SDLException("SDL_InitSubSystem(SDL_INIT_VIDEO) falla : ");

#ifdef LOG_SISTEMA
    printf("Iniciando Video Mode...\n");
#endif

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // 32 bits por pixel
//    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32); // TODO: Esta línea causa conflicto en Linux. Averiguar porque.

    // con double buffer

    if (doublebuff) {
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
    }

    Uint32 flags = SDL_OPENGL;

    if (fullscreen) 
        flags |= SDL_FULLSCREEN;

    if ((_impl->m_SDL_Surface = SDL_SetVideoMode ( width , height , bpp, flags)) == nullptr)
        throw Exception("SDL_SetVideoMode ( width , height , bpp, flags) falla ");

    _impl->m_FullScreen=fullscreen;
    _impl->m_Doublebuff=doublebuff;
    _impl->m_Width=width;
    _impl->m_Height=height;
    _impl->m_Bpp=bpp;

    // Set the OpenGL state after creating the context with SDL_SetVideoMode

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
        
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClearColor(0,0,0,0);

    SDL_ShowCursor( 0 );
    SDL_WM_SetCaption("biblioteca-opengl-2d",nullptr);

    return EXITO;

}

int Screen::start(int width, int height, int bpp, Float scalex, Float scaley, bool fullscreen, bool doublebuff) {

    if (_impl->m_SDL_Surface) throw Exception("Video ya inicializado, orden imposible 'start'");

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == -1) throw SDLException("SDL_InitSubSystem(SDL_INIT_VIDEO) falla ");

#ifdef LOG_SISTEMA
    printf("Iniciando Video Mode...\n");
#endif

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // 32 bits por pixel
//    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32); // TODO: Esta línea causa conflicto en Linux. Averiguar porque.

    // con double buffer

    if (doublebuff) {
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
    }

    Uint32 flags = SDL_OPENGL;

    if (fullscreen) 
        flags |= SDL_FULLSCREEN;

    if ((_impl->m_SDL_Surface= SDL_SetVideoMode ( width , height , bpp, flags)) == nullptr) {
        throw SDLException("SDL_SetVideoMode ( width , height , bpp, flags) falla ");

    _impl->m_FullScreen=fullscreen;
    _impl->m_Doublebuff=doublebuff;
    _impl->m_Width=width;
    _impl->m_Height=height;
    _impl->m_Bpp=bpp;


    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
        
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClearColor(0,0,0,0);

    SDL_ShowCursor( 0 );
    SDL_WM_SetCaption("biblioteca-opengl-2d",nullptr);

    return EXITO;

}

int Screen::render() {

    if (!_impl->m_SDL_Surface) throw Exception("Video context not inicialized");

#ifdef MAINRENDERLOOP

    _impl->rendering = true;

    _impl->beginRenderMode(RENDER_TEXTURE_STANDARD);

    std::list<ScreenImpl::SRender*>& graphicMaterial = _impl->graphicMaterial;

    for (std::list<ScreenImpl::SRender*>::iterator it = graphicMaterial.begin(), jt = graphicMaterial.end(); it != jt ; ++it) {
        ScreenImpl::SRender* r = *it;
        (*r)();
    }

    _impl->endRenderMode(RENDER_TEXTURE_STANDARD);

    graphicMaterial.clear();

    _impl->rendering = false;

#endif

    SDL_GL_SwapBuffers();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    _impl->deleteResources();

    return EXITO;
}

int Screen::clear ( ) 
{
    if (!_impl->m_SDL_Surface) throw Exception("Video context not inicialized");

#ifdef MAINRENDERLOOP

    auto & graphicMaterial = _impl->graphicMaterial;
    for (auto it = graphicMaterial.begin(), jt = graphicMaterial.end(); it != jt ; ++it) {
        (**it)();
    }

    graphicMaterial.clear();

#endif

    _impl->deleteResources();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();

    return EXITO;
}

Float Screen::getA() {
    return _impl->alpha;
}

Float Screen::getR() {
    return _impl->red;
}

Float Screen::getG() {
    return _impl->green;
}

Float Screen::getB() {
    return _impl->blue;
}

int Screen::locateRenderScene(Float posx, Float posy, Float width, Float height,Float zoom) {

    (width <= 0.0f)?  width  = Screen::I()._impl->m_Width  : 0.0f ;
    (height <= 0.0f)? height = Screen::I()._impl->m_Height : 0.0f ;

    ScreenImpl::SRenderLocation* rr = new ScreenImpl::SRenderLocation;
    rr->posx = posx;
    rr->posy = posy;
    rr->width = width;
    rr->height = height;
    rr->zoom = zoom;

#ifdef MAINRENDERLOOP
    _impl->graphicMaterial.push_back(rr);
#else
    (*rr)();
#endif

    return EXITO;

}

int Screen::rotate(Float angle, Float x, Float y, Float z) {

    ScreenImpl::SRenderRotation* rr = new ScreenImpl::SRenderRotation;
    rr->angle = angle;
    rr->x = x;
    rr->y = y;
    rr->z = z;

#ifdef MAINRENDERLOOP
    _impl->graphicMaterial.push_back(rr);
#else
    (*rr)();
#endif

    return EXITO;
}

int Screen::translate(Float x, Float y, Float z) {

    ScreenImpl::SRenderTranslation* rr = new ScreenImpl::SRenderTranslation;
    rr->x = x;
    rr->y = y;
    rr->z = z;

#ifdef MAINRENDERLOOP
    _impl->graphicMaterial.push_back(rr);
#else
    (*rr)();
#endif

    return EXITO;
}

int Screen::scale(Float x, Float y, Float z) {
    
    ScreenImpl::SRenderScalation* rr = new ScreenImpl::SRenderScalation;
    rr->x = x;
    rr->y = y;
    rr->z = z;

#ifdef MAINRENDERLOOP
    _impl->graphicMaterial.push_back(rr);
#else
    (*rr)();
#endif

    return EXITO;
}

int Screen::color(Float red, Float green, Float blue, Float alpha) {

    ScreenImpl::SRenderColor* rr = new ScreenImpl::SRenderColor;

    _impl->red = rr->red = red;
    _impl->green = rr->green = green;
    _impl->blue = rr->blue = blue;
    _impl->alpha = rr->alpha = alpha;

#ifdef MAINRENDERLOOP
    _impl->graphicMaterial.push_back(rr);
#else
    (*rr)();
#endif

    return EXITO;
}

int Screen::color(Color* col, Float alpha) {

    return color(((Float)col->getR())/255.0,((Float)col->getG())/255.0,((Float)col->getB())/255.0,_impl->alpha);

}

int Screen::projectionMode(TypeRendeProjection trp, Float zMax) {

    if (_impl->rendering) 
        throw Exception("No se puede cambiar el modo de proyección mientras se está en fase de renderización");

    _impl->m_maxZ = zMax;
    _impl->trp = trp;

    return EXITO;

}

int Screen::pushMatrix() {
    static ScreenImpl::SRenderPushMatrix rr;
#ifdef MAINRENDERLOOP
    _impl->graphicMaterial.push_back(static_cast<ScreenImpl::SRender*>(&rr));
#else
    rr();
#endif

    return EXITO;

}

int Screen::popMatrix() {
    static ScreenImpl::SRenderPopMatrix rr;
#ifdef MAINRENDERLOOP
    _impl->graphicMaterial.push_back(static_cast<ScreenImpl::SRender*>(&rr));
#else
    rr();
#endif

    return EXITO;

}

int Screen::ScreenImpl::beginRenderMode(Uint32 flags) {
    if (!m_SDL_Surface) throw Exception("Video context not inicialized");
    
    if (flags & RENDER_TEXTURE_STANDARD) {
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    return EXITO;
}

int Screen::ScreenImpl::endRenderMode(Uint32 flags) {
    if (!m_SDL_Surface) throw Exception("Video context not inicialized");

    if (flags & RENDER_TEXTURE_STANDARD) {
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_COLOR_MATERIAL);
    }

    return EXITO;
}


int Screen::quit()
{
    if (!_impl->m_SDL_Surface) throw Exception("Video context not inicialized");

    SDL_FreeSurface(_impl->m_SDL_Surface);
    _impl->m_SDL_Surface = nullptr;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    return EXITO;
}

Uint8 Screen::getBpp() {
    return _impl->m_Bpp;
}

int Screen::getWidth() {
    return _impl->m_Width;
}

int Screen::getHeight() {
    return _impl->m_Height;
}

bool Screen::isFullscreen() {
    return _impl->m_FullScreen;
}

int Screen::changeScreen(int width, int height, int bpp, Float scalex, Float scaley, bool fullscreen) {

    if (!_impl->m_SDL_Surface) throw Exception("Video context not inicialized");

    clear();
    
    GraphicResources info;

    _impl->saveResources(info);

    quit();

    if ( start (width,height,bpp,scalex, scaley,fullscreen,_impl->m_Doublebuff) == FRACASO)
        return FRACASO;

    _impl->reloadResources(info);

    return EXITO;
    
}

int Screen::ToggleFullscreen() {

    if (!_impl->m_SDL_Surface) throw Exception("Video context not inicialized");

    clear();

    GraphicResources info;

    _impl->saveResources(info);

    quit();

    _impl->m_FullScreen = !_impl->m_FullScreen;

    if ( start (_impl->m_Width,_impl->m_Height,_impl->m_Bpp,_impl->m_FullScreen,_impl->m_Doublebuff) == FRACASO )
        return FRACASO;

    _impl->reloadResources(info);

    return EXITO;

}

int Screen::setDoublebuffer(bool doublebuff) {

    if (!_impl->m_SDL_Surface) throw Exception("Video context not inicialized");

    if (doublebuff!=_impl->m_Doublebuff) {
        clear();

        GraphicResources info;

        _impl->saveResources(info);

        quit();

        if ( start (_impl->m_Width,_impl->m_Height,_impl->m_Bpp,_impl->m_FullScreen,doublebuff) == FRACASO)
            return FRACASO;

        _impl->reloadResources(info);
    }

    return EXITO;
}

#ifdef GLOBAL_SINGLETON_REFERENCES
Screen& FSDraw = Screen::I();
#endif

void Screen::ScreenImpl::SRenderLocation::operator()() {
    //glViewport(posx*m_ScaleX,posy*m_ScaleY,width*m_ScaleX,height*m_ScaleY);
    glViewport((int)posx,(int)posy,(int)width,(int)height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    static const TypeRendeProjection& trp = Screen::I()._impl->trp;

    if (trp == TRP_ORTHO) {

        //Opción ortogonal
        //glOrtho( 0.0, (double)width*m_ScaleX, (double)height*m_ScaleY, 0.0, 0.0, 1.0 ); //Los 2 últimos valores son la profundidad, sustituir por -100.0 y 100.0 para darle algo.

        glOrtho( 0.0, (double)width, (double)height, 0.0, 0.0, 1.0 ); //Los 2 últimos valores son la profundidad, sustituir por -100.0 y 100.0 para darle algo.

    } else {

        static const Float& m_maxZ = Screen::I()._impl->m_maxZ;

        //Opción de perspectiva 1
        gluPerspective(90.0f,width/height,1.0,m_maxZ);
        glTranslate(-width/2,height/2,-240.0);
        glRotate(180.0,1.0,0.0,0.0);

        //Opción de perspectiva 2
        //gluPerspective(60.0f,width/height,1.0,400.0);
        //glTranslatef(0.0,0.0,-205.0);
        //glTranslatef(0.0,0.0,101.0);
        //glRotatef(180.0,1.0,0.0,0.0);
        //glTranslatef(-width/2.0,-height/2.0,0.0);

        //Opción de perspectiva 3
        //gluPerspective(45.0f,width/height,1.0,400.0);
        //glTranslatef(0.0,0.0,-290.0);
        //glRotatef(180.0,1.0,0.0,0.0);
        //glTranslatef(-width/2.0,-height/2.0,0.0);

        //glRotatef(-45.0,1.0,0.0,0.0); //Ejemplo de rotación del plano en perspectiva
        //glTranslatef(0.0,-100.0,120.0);
    }
    delete this;
}

void Screen::ScreenImpl::SRenderTranslation::operator()() {
    glTranslate(x,y,z);
    delete this;
}

void Screen::ScreenImpl::SRenderScalation::operator()() {
    glScale(x,y,z);
    delete this;
}

void Screen::ScreenImpl::SRenderRotation::operator()() {
    glRotate(angle,x,y,z);
    delete this;
}

void Screen::ScreenImpl::SRenderColor::operator()() {
    glColor4(red,green,blue,alpha);
    delete this;
}

void Screen::ScreenImpl::SRenderPushMatrix::operator()() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void Screen::ScreenImpl::SRenderPopMatrix::operator()() {
    glPopMatrix();
}

void Screen::ScreenImpl::SRenderCanvas::operator()() {
    glBindTexture(GL_TEXTURE_2D, tex);

    glBegin(GL_QUADS);
    switch(flags) {
        case 0:
            glTexCoord2(0.0f, relH);
            glVertex2(0, h2);
            glTexCoord2(relW, relH);
            glVertex2(w2, h2);
            glTexCoord2(relW, 0.0f);
            glVertex2(w2, 0);
            glTexCoord2(0.0f, 0.0f);
            glVertex2(0,0);
            break;
        case 1:
            glTexCoord2(relW, relH);
            glVertex2(0, h2);
            glTexCoord2(0.0f, relH);
            glVertex2(w2, h2);
            glTexCoord2(0.0f, 0.0f);
            glVertex2(w2, 0);
            glTexCoord2(relW, 0.0f);
            glVertex2(0,0);
            break;
        case 2:
            glTexCoord2(0.0f, 0.0f);
            glVertex2(0, h2);
            glTexCoord2(relW, 0.0f);
            glVertex2(w2, h2);
            glTexCoord2(relW, relH);
            glVertex2(w2, 0);
            glTexCoord2(0.0f, relH);
            glVertex2(0,0);
            break;
        case 3:
            glTexCoord2(relW, 0.0f);
            glVertex2(0, h2);
            glTexCoord2(0.0f, 0.0f);
            glVertex2(w2, h2);
            glTexCoord2(0.0f, relH);
            glVertex2(w2, 0);
            glTexCoord2(relW, relH);
            glVertex2(0,0);
            break;
        default:
            break;
    }
    glEnd();

    delete this;
}

} // flatscene
