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

    _impl->initProcRenders();
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
//	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32); // TODO: Esta línea causa conflicto en Linux. Averiguar porque.

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
//	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32); // TODO: Esta línea causa conflicto en Linux. Averiguar porque.

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

void FSScreen::ScreenImpl::initProcRenders() {

	procRenders[TR_CANVAS] = ScreenImpl::procRendCanvas;
	procRenders[TR_FLOATCANVAS] = ScreenImpl::procRendFloatCanvas;
	procRenders[TR_ROTATION] = ScreenImpl::procRendRotation;
	procRenders[TR_TRANSLATION] = ScreenImpl::procRendTranslation;
	procRenders[TR_LOCATION] = ScreenImpl::procRendLocation;
	procRenders[TR_PUSHMATRIX] = ScreenImpl::procRendPush;
	procRenders[TR_POPMATRIX] = ScreenImpl::procRendPop;
	procRenders[TR_SCALATION] = ScreenImpl::procRendScalation;
	procRenders[TR_COLOR] = ScreenImpl::procRendColor;
}

int FSScreen::render ( ) 
{
	if (!_impl->m_SDL_Surface) {
		FSLibrary::I().Error("Video context not inicialized");
		return false;
	}

#ifdef MAINRENDERLOOP

	_impl->rendering = true;

	_impl->beginRenderMode(RENDER_TEXTURE_STANDARD);

    const map<TypeResource,void (*)(void*)>& procRenders = _impl->procRenders;
    list<SToRender*>& graphicMaterial = _impl->graphicMaterial;

	for (list<SToRender*>::iterator it = graphicMaterial.begin(), jt = graphicMaterial.end(); it != jt ; ++it) {

		SToRender* em =(*it);

		procRenders.at(em->type)(em->pointer);

		delete *it;
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

	list<SToRender*>& graphicMaterial = _impl->graphicMaterial;
	for (list<SToRender*>::iterator it = graphicMaterial.begin(), jt = graphicMaterial.end(); it != jt ; ++it) {
		SToRender* em = *it;

		if (em->type == TR_CANVAS)
			delete ((SRenderCanvas<FSPoint>*)em->pointer);
		else if (em->type == TR_FLOATCANVAS)
			delete ((SRenderCanvas<FSFloatPoint>*)em->pointer);
		else if (em->type == TR_LOCATION)
			delete ((SRenderLocation*)em->pointer);
		else if (em->type == TR_ROTATION)
			delete ((SRenderRotation*)em->pointer);
		else if (em->type == TR_SCALATION || em->type == TR_TRANSLATION)
			delete ((SRenderTranscalation*)em->pointer);

		delete *it;
	}

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

#ifdef MAINRENDERLOOP

    (width <= 0.0f)?  width  = FSScreen::I()._impl->m_Width  : 0 ;
    (height <= 0.0f)? height = FSScreen::I()._impl->m_Height : 0 ;

	SRenderLocation* n = new SRenderLocation;
	n->posx = posx;
	n->posy = posy;
	n->width = width;
	n->height = height;
	n->zoom = zoom;

	SToRender* em = new SToRender;

	em->type = TR_LOCATION;
	em->pointer = (void*) n;

	_impl->graphicMaterial.push_back(em);

#else

	glViewport(posx*m_ScaleX,posy*m_ScaleY,width*m_ScaleX,height*m_ScaleY);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//Opción ortogonal
	glOrtho( 0.0, (double)width*m_ScaleX, (double)height*m_ScaleY, 0.0, 0.0, 1.0 ); //Los 2 últimos valores son la profundidad, sustituir por -100.0 y 100.0 para darle algo.
	glScalef(m_ScaleX*zoom,m_ScaleY*zoom,1.0);

	//Opción de perspectiva 1
	//gluPerspective(90.0f,width/height,1.0,400.0);
	//glTranslatef(0.0,0.0,-120.0);
	//glRotatef(180.0,1.0,0.0,0.0);
	//glTranslatef(-width/2.0,-height/2.0,0.0);

	//Opción de perspectiva 2
	//gluPerspective(60.0f,width/height,1.0,400.0);
	//glTranslatef(0.0,0.0,-205.0);
	//glRotatef(180.0,1.0,0.0,0.0);
	//glTranslatef(-width/2.0,-height/2.0,0.0);

	//Opción de perspectiva 3
	//gluPerspective(45.0f,width/height,1.0,400.0);
	//glTranslatef(0.0,0.0,-290.0);
	//glRotatef(180.0,1.0,0.0,0.0);
	//glTranslatef(-width/2.0,-height/2.0,0.0);

	//glRotatef(-45.0,1.0,0.0,0.0); //Ejemplo de rotación del plano en perspectiva
	//glTranslatef(0.0,-100.0,120.0);

#endif

	return EXITO;

}

int FSScreen::rotate(float angle, float x, float y, float z) {


#ifdef MAINRENDERLOOP

	SRenderRotation* n = new SRenderRotation;
	n->angle = angle;
	n->x = x;
	n->y = y;
	n->z = z;

	SToRender* em = new SToRender;

	em->type = TR_ROTATION;
	em->pointer = (void*) n;

	_impl->graphicMaterial.push_back(em);

#else

	glRotatef(angle,x,y,z);

#endif

	return EXITO;
}

int FSScreen::translate(float x, float y, float z) {

#ifdef MAINRENDERLOOP

	SRenderTranscalation* n = new SRenderTranscalation;
	n->x = x;
	n->y = y;
	n->z = z;

	SToRender* em = new SToRender;

	em->type = TR_TRANSLATION;
	em->pointer = (void*) n;

	_impl->graphicMaterial.push_back(em);

#else

	glTranslatef(x,y,z);

#endif

	return EXITO;
}

int FSScreen::scale(float x, float y, float z) {

#ifdef MAINRENDERLOOP

	SRenderTranscalation* n = new SRenderTranscalation;
	n->x = x;
	n->y = y;
	n->z = z;

	SToRender* em = new SToRender;

	em->type = TR_SCALATION;
	em->pointer = (void*) n;

	_impl->graphicMaterial.push_back(em);

#else

	glScalef(x,y,z);

#endif

	return EXITO;
}

int FSScreen::color(float red, float green, float blue, float alpha) {

#ifdef MAINRENDERLOOP

	SRenderColor* n = new SRenderColor;

	_impl->red = n->red = red;
	_impl->green = n->green = green;
	_impl->blue = n->blue = blue;
	_impl->alpha = n->alpha = alpha;

	SToRender* em = new SToRender;

	em->type = TR_COLOR;
	em->pointer = (void*) n;

	_impl->graphicMaterial.push_back(em);

#else

	glColor4f(red,green,blue,alpha);

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

#ifdef MAINRENDERLOOP

	SToRender* em = new SToRender;

	em->type = TR_PUSHMATRIX;
	em->pointer = NULL;

	_impl->graphicMaterial.push_back(em);

#else

	glPushMatrix();

#endif

	return EXITO;

}

int FSScreen::popMatrix() {

#ifdef MAINRENDERLOOP

	SToRender* em = new SToRender;

	em->type = TR_POPMATRIX;
	em->pointer = NULL;

	_impl->graphicMaterial.push_back(em);

#else

	glPopMatrix();

#endif

	return EXITO;

}



void FSScreen::ScreenImpl::procRendCanvas(void* pointer) {

	SRenderCanvas<FSPoint>* n = (SRenderCanvas<FSPoint>*) pointer;

	SCanvas m_pSurface = n->canvas;
	Uint8 flags = n->flags;
	FSPoint ptDst = n->ptDst;

	delete n;

	if (m_pSurface.h != 0 || m_pSurface.w !=0 ) {

		glBindTexture(GL_TEXTURE_2D, m_pSurface.tex); 

		float relW = (float)m_pSurface.w2/(float)m_pSurface.w;
		float relH = (float)m_pSurface.h2/(float)m_pSurface.h;

		//glScalef((1.0/m_ScaleX ),(1.0/m_ScaleY ),0.0);

		glBegin(GL_QUADS);
			if (flags == 0) {
				glTexCoord2f(0.0f, relH);
				glVertex2f(0, m_pSurface.h2);      
				glTexCoord2f(relW, relH);
				glVertex2f(m_pSurface.w2, m_pSurface.h2);
				glTexCoord2f(relW, 0.0f);
				glVertex2f(m_pSurface.w2, 0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0,0);
			} else if (flags == 1) {
				
				glTexCoord2f(relW, relH);
				glVertex2f(0, m_pSurface.h2);      
				glTexCoord2f(0.0f, relH);		
				glVertex2f(m_pSurface.w2, m_pSurface.h2);
				glTexCoord2f(0.0f, 0.0f);						
				glVertex2f(m_pSurface.w2, 0);
				glTexCoord2f(relW, 0.0f);
				glVertex2f(0,0);
			} else if (flags==2) {
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0, m_pSurface.h2);      
				glTexCoord2f(relW, 0.0f);
				glVertex2f(m_pSurface.w2, m_pSurface.h2);
				glTexCoord2f(relW, relH);
				glVertex2f(m_pSurface.w2, 0);
				glTexCoord2f(0.0f, relH);	
				glVertex2f(0,0);
			} else  {
				glTexCoord2f(relW, 0.0f);
				glVertex2f(0, m_pSurface.h2);      
				glTexCoord2f(0.0f, 0.0f);		
				glVertex2f(m_pSurface.w2, m_pSurface.h2);
				glTexCoord2f(0.0f, relH);						
				glVertex2f(m_pSurface.w2, 0);
				glTexCoord2f(relW, relH);
				glVertex2f(0,0);
			} 
		glEnd();

	}

}

void FSScreen::ScreenImpl::procRendFloatCanvas(void* pointer) {

	SRenderCanvas<FSFloatPoint>* n = (SRenderCanvas<FSFloatPoint>*) pointer;

	SCanvas m_pSurface = (n->canvas);
	Uint8 flags = n->flags;
	FSFloatPoint ptDst = n->ptDst;

	delete n;

	if (m_pSurface.h != 0 || m_pSurface.w !=0 ) {

		glBindTexture(GL_TEXTURE_2D, m_pSurface.tex); 

		float relW = (float)m_pSurface.w2/(float)m_pSurface.w;
		float relH = (float)m_pSurface.h2/(float)m_pSurface.h;

		//glScalef((1.0/m_ScaleX ),(1.0/m_ScaleY ),0.0);

		glBegin(GL_QUADS);
			if (flags == 0) {
				glTexCoord2f(0.0f, relH);
				glVertex2f(0, m_pSurface.h2);      
				glTexCoord2f(relW, relH);
				glVertex2f(m_pSurface.w2, m_pSurface.h2);
				glTexCoord2f(relW, 0.0f);
				glVertex2f(m_pSurface.w2, 0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0,0);
			} else if (flags == 1) {
				
				glTexCoord2f(relW, relH);
				glVertex2f(0, m_pSurface.h2);      
				glTexCoord2f(0.0f, relH);		
				glVertex2f(m_pSurface.w2, m_pSurface.h2);
				glTexCoord2f(0.0f, 0.0f);						
				glVertex2f(m_pSurface.w2, 0);
				glTexCoord2f(relW, 0.0f);
				glVertex2f(0,0);
			} else if (flags==2) {
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0, m_pSurface.h2);      
				glTexCoord2f(relW, 0.0f);
				glVertex2f(m_pSurface.w2, m_pSurface.h2);
				glTexCoord2f(relW, relH);
				glVertex2f(m_pSurface.w2, 0);
				glTexCoord2f(0.0f, relH);	
				glVertex2f(0,0);
			} else  {
				glTexCoord2f(relW, 0.0f);
				glVertex2f(0, m_pSurface.h2);      
				glTexCoord2f(0.0f, 0.0f);		
				glVertex2f(m_pSurface.w2, m_pSurface.h2);
				glTexCoord2f(0.0f, relH);						
				glVertex2f(m_pSurface.w2, 0);
				glTexCoord2f(relW, relH);
				glVertex2f(0,0);
			} 
		glEnd();

	}


}
void FSScreen::ScreenImpl::procRendRotation(void* pointer) {

	SRenderRotation* n = (SRenderRotation*) pointer;

	GLfloat angle = n->angle;
	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;

	glRotatef(angle,x,y,z);

}

void FSScreen::ScreenImpl::procRendTranslation(void* pointer) {


	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;
			
	glTranslatef(x,y,z);


}

void FSScreen::ScreenImpl::procRendLocation(void* pointer) {

	SRenderLocation* n = (SRenderLocation*)pointer;

	float posx = n->posx;
	float posy = n->posy;
	float width = n->width;
	float height = n->height;
	float zoom = n->zoom;

	delete n;

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


}

void FSScreen::ScreenImpl::procRendPush(void* pointer) {

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

}

void FSScreen::ScreenImpl::procRendPop(void* pointer) {

	glPopMatrix();

}

void FSScreen::ScreenImpl::procRendScalation(void* pointer) {

	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;

	glScalef(x ,y ,z);

}

void FSScreen::ScreenImpl::procRendColor(void* pointer) {

	SRenderColor* n = (SRenderColor*) pointer;

	GLfloat red = n->red;
	GLfloat green = n->green;
	GLfloat blue = n->blue;
	GLfloat alpha = n->alpha;

	delete n;

	glColor4f(red,green,blue,alpha);

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
