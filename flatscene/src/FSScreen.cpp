#include "FSScreenImpl.h"
#include "FSLibrary.h"
#include "FSEngine.h"

FSScreen::FSScreen() : _impl(new ScreenImpl) {
    _impl->m_SDL_Surface=NULL;

    _impl->m_FullScreen=false;
    _impl->m_Doublebuff=true;
    _impl->rendering=false;
    _impl->m_maxZ=400.0f;

    _impl->trp=TRP_PERSPECTIVE;

    _impl->m_Bpp=0;
    _impl->m_Width=0;
    _impl->m_Height=0;

    _impl->alpha =1.0f;
    _impl->red =1.0f;
    _impl->green =1.0f;
    _impl->blue =1.0f;
}

FSScreen::~FSScreen() {
    delete _impl;
}

int FSScreen::start(int width, int height, int bpp, bool fullscreen, bool doublebuff)
{ 
    	
    if (_impl->m_SDL_Surface) {
    	FSLibrary::I().Error("Video ya inicializado, orden imposible 'start'\n"); 
    	return FRACASO;
    }

    if (SDL_InitSubSystem(SDL_INIT_VIDEO)==-1) {
    	FSLibrary::I().Error("SDL_InitSubSystem(SDL_INIT_VIDEO) falla : ",TE_SDL_MSG);
    	return FRACASO;
    }
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

    if ((_impl->m_SDL_Surface= SDL_SetVideoMode ( width , height , bpp, flags))==NULL) {
    	FSLibrary::I().Error("SDL_SetVideoMode ( width , height , bpp, flags) falla : ",TE_SDL_MSG);
    	return FRACASO;
    }

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
    SDL_WM_SetCaption("biblioteca-opengl-2d",NULL);

    return EXITO;

}

int FSScreen::start(int width, int height, int bpp, float scalex, float scaley, bool fullscreen, bool doublebuff)
{ 
    	
    if (_impl->m_SDL_Surface) {
    	FSLibrary::I().Error("Video ya inicializado, orden imposible 'start'\n"); 
    	return FRACASO;
    }

    if (SDL_InitSubSystem(SDL_INIT_VIDEO)==-1) {
    	FSLibrary::I().Error("SDL_InitSubSystem(SDL_INIT_VIDEO) falla : ",TE_SDL_MSG);
    	return FRACASO;
    }
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

    if ((_impl->m_SDL_Surface= SDL_SetVideoMode ( width , height , bpp, flags))==NULL) {
    	FSLibrary::I().Error("SDL_SetVideoMode ( width , height , bpp, flags) falla : ",TE_SDL_MSG);
    	return FRACASO;
    }

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
    SDL_WM_SetCaption("biblioteca-opengl-2d",NULL);

    return EXITO;

}

int FSScreen::render() 
{
    if (!_impl->m_SDL_Surface) {
    	FSLibrary::I().Error("Video context not inicialized");
    	return false;
    }

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

int FSScreen::clear ( ) 
{
    if (!_impl->m_SDL_Surface) {
    	FSLibrary::I().Error("Video context not inicialized");
    	return FRACASO;
    }

#ifdef MAINRENDERLOOP

<<<<<<< Updated upstream
    list<ScreenImpl::SRender*>& graphicMaterial = _impl->graphicMaterial;
    for (list<ScreenImpl::SRender*>::iterator it = graphicMaterial.begin(), jt = graphicMaterial.end(); it != jt ; ++it) {
    	(**it)();
    }
=======
	std::list<ScreenImpl::SRender*>& graphicMaterial = _impl->graphicMaterial;
	for (std::list<ScreenImpl::SRender*>::iterator it = graphicMaterial.begin(), jt = graphicMaterial.end(); it != jt ; ++it) {
		(**it)();
	}
>>>>>>> Stashed changes

    graphicMaterial.clear();

#endif

    _impl->deleteResources();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    glLoadIdentity();	

    return EXITO;
}

float FSScreen::getA() {
    return _impl->alpha;
}

float FSScreen::getR() {
    return _impl->red;
}

float FSScreen::getG() {
    return _impl->green;
}

float FSScreen::getB() {
    return _impl->blue;
}

int FSScreen::locateRenderScene(float posx, float posy, float width, float height,float zoom) {

    (width <= 0.0f)?  width  = FSScreen::I()._impl->m_Width  : 0 ;
    (height <= 0.0f)? height = FSScreen::I()._impl->m_Height : 0 ;

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

int FSScreen::rotate(float angle, float x, float y, float z) {

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

int FSScreen::translate(float x, float y, float z) {

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

int FSScreen::scale(float x, float y, float z) {
    
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

int FSScreen::color(float red, float green, float blue, float alpha) {

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

int FSScreen::color(FSColor* col, float alpha) {

    return color(((float)col->getR())/255.0,((float)col->getG())/255.0,((float)col->getB())/255.0,_impl->alpha);

}

int FSScreen::projectionMode(TypeRendeProjection trp, float zMax) {

    if (_impl->rendering) {
    	FSLibrary::I().Error("No se puede cambiar el modo de proyección mientras se está en fase de renderización");
    	return FRACASO;
    }

    _impl->m_maxZ = zMax;
    _impl->trp = trp;

    return EXITO;

}

int FSScreen::pushMatrix() {
    static ScreenImpl::SRenderPushMatrix rr;
#ifdef MAINRENDERLOOP
    _impl->graphicMaterial.push_back(static_cast<ScreenImpl::SRender*>(&rr));
#else
    rr();
#endif

    return EXITO;

}

int FSScreen::popMatrix() {
    static ScreenImpl::SRenderPopMatrix rr;
#ifdef MAINRENDERLOOP
    _impl->graphicMaterial.push_back(static_cast<ScreenImpl::SRender*>(&rr));
#else
    rr();
#endif

    return EXITO;

}

int FSScreen::ScreenImpl::beginRenderMode(Uint32 flags) {
    if (!m_SDL_Surface) {
    	FSLibrary::I().Error("Video context not inicialized");
    	return FRACASO;
    }
    
    if (flags & RENDER_TEXTURE_STANDARD) {
    	glEnable(GL_COLOR_MATERIAL);
    	glEnable(GL_TEXTURE_2D);
    	glEnable(GL_BLEND);
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    }

    return EXITO;
}

int FSScreen::ScreenImpl::endRenderMode(Uint32 flags) {
    if (!m_SDL_Surface) {
    	FSLibrary::I().Error("Video context not inicialized");
    	return FRACASO;
    }

    if (flags & RENDER_TEXTURE_STANDARD) {
    	glDisable(GL_BLEND);
    	glDisable(GL_TEXTURE_2D);
    	glDisable(GL_COLOR_MATERIAL);
    }

    return EXITO;
}


int FSScreen::quit()
{
    if (!_impl->m_SDL_Surface) {
    	FSLibrary::I().Error("Video context not inicialized");
    	return FRACASO;
    }

    SDL_FreeSurface(_impl->m_SDL_Surface);
    _impl->m_SDL_Surface=NULL;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    return EXITO;
}

Uint8 FSScreen::getBpp() {
    return _impl->m_Bpp;
}

int FSScreen::getWidth() {
    return _impl->m_Width;
}

int FSScreen::getHeight() {
    return _impl->m_Height;
}

bool FSScreen::isFullscreen() {
    return _impl->m_FullScreen;
}

int FSScreen::changeScreen(int width, int height, int bpp, float scalex, float scaley, bool fullscreen) {
    if (!_impl->m_SDL_Surface) {
    	FSLibrary::I().Error("Video context not inicialized");
    	return FRACASO;
    }
    

    clear();
    
    GraphicResources info;

    _impl->saveResources(info);

    quit();

    if ( start (width,height,bpp,scalex, scaley,fullscreen,_impl->m_Doublebuff) == FRACASO)
    	return FRACASO;

    _impl->reloadResources(info);

    return EXITO;
    
}

int FSScreen::ToggleFullscreen() {

    if (!_impl->m_SDL_Surface) {
    	FSLibrary::I().Error("Video context not inicialized");
    	return FRACASO;
    }

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

int FSScreen::setDoublebuffer(bool doublebuff) {

    if (!_impl->m_SDL_Surface) {
    	FSLibrary::I().Error("Video context not inicialized");
    	return FRACASO;
    }

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
FSScreen& FSDraw = FSScreen::I();
#endif

void FSScreen::ScreenImpl::SRenderLocation::operator()() {
    //glViewport(posx*m_ScaleX,posy*m_ScaleY,width*m_ScaleX,height*m_ScaleY);
    glViewport(posx,posy,width,height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    static const TypeRendeProjection& trp = FSScreen::I()._impl->trp;

    if (trp == TRP_ORTHO) {

    	//Opción ortogonal
    	//glOrtho( 0.0, (double)width*m_ScaleX, (double)height*m_ScaleY, 0.0, 0.0, 1.0 ); //Los 2 últimos valores son la profundidad, sustituir por -100.0 y 100.0 para darle algo.

    	glOrtho( 0.0, (double)width, (double)height, 0.0, 0.0, 1.0 ); //Los 2 últimos valores son la profundidad, sustituir por -100.0 y 100.0 para darle algo.

    } else {

        static const float& m_maxZ = FSScreen::I()._impl->m_maxZ;

    	//Opción de perspectiva 1
    	gluPerspective(90.0f,width/height,1.0,m_maxZ);
    	glTranslatef(-width/2,height/2,-240.0);
    	glRotatef(180.0,1.0,0.0,0.0);

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

void FSScreen::ScreenImpl::SRenderTranslation::operator()() {
    glTranslatef(x,y,z);
    delete this;
}

void FSScreen::ScreenImpl::SRenderScalation::operator()() {
    glScalef(x,y,z);
    delete this;
}

void FSScreen::ScreenImpl::SRenderRotation::operator()() {
    glRotatef(angle,x,y,z);
    delete this;
}

void FSScreen::ScreenImpl::SRenderColor::operator()() {
    glColor4f(red,green,blue,alpha);
    delete this;
}

void FSScreen::ScreenImpl::SRenderPushMatrix::operator()() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void FSScreen::ScreenImpl::SRenderPopMatrix::operator()() {
    glPopMatrix();
}
