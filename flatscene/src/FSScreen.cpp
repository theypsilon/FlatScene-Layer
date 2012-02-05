#include "FSScreenImpl.h"
#include "FSLibrary.h"
#include "FSEngine.h"

FSScreen::FSScreen() : _impl(new ScreenImpl) {}

FSScreen::~FSScreen() {
    delete _impl;
}

SDL_Surface* FSScreen::m_SDL_Surface=NULL;

bool FSScreen::m_FullScreen=false;
bool FSScreen::m_Doublebuff=true;
bool FSScreen::rendering=false;
float FSScreen::m_maxZ=400.0;

TypeRendeProjection FSScreen::trp=TRP_PERSPECTIVE;

int FSScreen::m_Bpp=0;
int FSScreen::m_Width=0;
int FSScreen::m_Height=0;

float FSScreen::alpha =1.0;
float FSScreen::red =1.0;
float FSScreen::green =1.0;
float FSScreen::blue =1.0;

list<SToRender*> FSScreen::graphicMaterial;

list<FSSprite*> FSScreen::spriteToDelete;
list<FSSpriteset*> FSScreen::spritesetToDelete;
list<FSCanvas*> FSScreen::imageToDelete;

map<TypeResource,void (*)(void*)> FSScreen::procRenders;

int FSScreen::start(int width, int height, int bpp, bool fullscreen, bool doublebuff)
{ 
		
	if (m_SDL_Surface) {
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

	if ((m_SDL_Surface= SDL_SetVideoMode ( width , height , bpp, flags))==NULL) {
		FSLibrary::I().Error("SDL_SetVideoMode ( width , height , bpp, flags) falla : ",TE_SDL_MSG);
		return FRACASO;
	}

	m_FullScreen=fullscreen;
	m_Doublebuff=doublebuff;
	m_Width=width;
	m_Height=height;
	m_Bpp=bpp;

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
		
	if (m_SDL_Surface) {
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

	if ((m_SDL_Surface= SDL_SetVideoMode ( width , height , bpp, flags))==NULL) {
		FSLibrary::I().Error("SDL_SetVideoMode ( width , height , bpp, flags) falla : ",TE_SDL_MSG);
		return FRACASO;
	}

	m_FullScreen=fullscreen;
	m_Doublebuff=doublebuff;
	m_Width=width;
	m_Height=height;
	m_Bpp=bpp;


	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	    
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClearColor(0,0,0,0);

	SDL_ShowCursor( 0 );
	SDL_WM_SetCaption("biblioteca-opengl-2d",NULL);

	return EXITO;

}

void FSScreen::initProcRenders() {

	procRenders[TR_CANVAS] = procRendCanvas;
	procRenders[TR_FLOATCANVAS] = procRendFloatCanvas;
	procRenders[TR_ROTATION] = procRendRotation;
	procRenders[TR_TRANSLATION] = procRendTranslation;
	procRenders[TR_LOCATION] = procRendLocation;
	procRenders[TR_PUSHMATRIX] = procRendPush;
	procRenders[TR_POPMATRIX] = procRendPop;
	procRenders[TR_SCALATION] = procRendScalation;
	procRenders[TR_COLOR] = procRendColor;
}

int FSScreen::render ( ) 
{
	if (!m_SDL_Surface) {
		FSLibrary::I().Error("Video context not inicialized");
		return false;
	}

#ifdef MAINRENDERLOOP

	rendering = true;

	beginRenderMode(RENDER_TEXTURE_STANDARD);

	for (list<SToRender*>::iterator it = graphicMaterial.begin(), jt = graphicMaterial.end(); it != jt ; ++it) {

		SToRender* em =(*it);

		procRenders[em->type](em->pointer);
		
		delete *it;
	}

	endRenderMode(RENDER_TEXTURE_STANDARD);

	graphicMaterial.clear();

	rendering = false;

#endif

	SDL_GL_SwapBuffers();
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();	

	deleteResources();

	return EXITO;
}

int FSScreen::clear ( ) 
{
	if (!m_SDL_Surface) {
		FSLibrary::I().Error("Video context not inicialized");
		return FRACASO;
	}

#ifdef MAINRENDERLOOP

	for (list<SToRender*>::iterator it = graphicMaterial.begin(), jt = graphicMaterial.end(); it != jt ; ++it) {
		SToRender* em = *it;

		if (em->type == TR_CANVAS)
			delete ((SRenderCanvas*)em->pointer);
		else if (em->type == TR_FLOATCANVAS)
			delete ((SRenderFloatCanvas*)em->pointer);
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

	deleteResources();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();	

	return EXITO;
}

float FSScreen::getA() {
	return alpha;
}

float FSScreen::getR() {
	return red;
}

float FSScreen::getG() {
	return green;
}

float FSScreen::getB() {
	return blue;
}

int FSScreen::locateRenderScene(float posx, float posy, float width, float height,float zoom) {

#ifdef MAINRENDERLOOP

	SRenderLocation* n = new SRenderLocation;
	n->posx = posx;
	n->posy = posy;
	n->width = width;
	n->height = height;
	n->zoom = zoom;

	SToRender* em = new SToRender;

	em->type = TR_LOCATION;
	em->pointer = (void*) n;

	graphicMaterial.push_back(em);

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

	graphicMaterial.push_back(em);

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

	graphicMaterial.push_back(em);

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

	graphicMaterial.push_back(em);

#else

	glScalef(x,y,z);

#endif

	return EXITO;
}

int FSScreen::color(float red, float green, float blue, float alpha) {

#ifdef MAINRENDERLOOP

	SRenderColor* n = new SRenderColor;

	FSScreen::red = n->red = red;
	FSScreen::green = n->green = green;
	FSScreen::blue = n->blue = blue;
	FSScreen::alpha = n->alpha = alpha;

	SToRender* em = new SToRender;

	em->type = TR_COLOR;
	em->pointer = (void*) n;

	graphicMaterial.push_back(em);

#else

	glColor4f(red,green,blue,alpha);

#endif

	return EXITO;
}

int FSScreen::color(FSColor* col, float alpha) {

	return color(((float)col->getR())/255.0,((float)col->getG())/255.0,((float)col->getB())/255.0,alpha);

}

int FSScreen::projectionMode(TypeRendeProjection trp, float zMax) {

	if (rendering) {
		FSLibrary::I().Error("No se puede cambiar el modo de proyección mientras se está en fase de renderización");
		return FRACASO;
	}

	m_maxZ = zMax;
	FSScreen::trp = trp;

	return EXITO;

}

int FSScreen::pushMatrix() {

#ifdef MAINRENDERLOOP

	SToRender* em = new SToRender;

	em->type = TR_PUSHMATRIX;
	em->pointer = NULL;

	graphicMaterial.push_back(em);

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

	graphicMaterial.push_back(em);

#else

	glPopMatrix();

#endif

	return EXITO;

}



void FSScreen::procRendCanvas(void* pointer) {

	SRenderCanvas* n = (SRenderCanvas*) pointer;

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

void FSScreen::procRendFloatCanvas(void* pointer) {

	SRenderFloatCanvas* n = (SRenderFloatCanvas*) pointer;

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
void FSScreen::procRendRotation(void* pointer) {

	SRenderRotation* n = (SRenderRotation*) pointer;

	GLfloat angle = n->angle;
	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;

	glRotatef(angle,x,y,z);

}

void FSScreen::procRendTranslation(void* pointer) {


	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;
			
	glTranslatef(x,y,z);


}

void FSScreen::procRendLocation(void* pointer) {

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

	if (trp == TRP_ORTHO) {

		//Opción ortogonal
		//glOrtho( 0.0, (double)width*m_ScaleX, (double)height*m_ScaleY, 0.0, 0.0, 1.0 ); //Los 2 últimos valores son la profundidad, sustituir por -100.0 y 100.0 para darle algo.

		glOrtho( 0.0, (double)width, (double)height, 0.0, 0.0, 1.0 ); //Los 2 últimos valores son la profundidad, sustituir por -100.0 y 100.0 para darle algo.

	} else {

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

void FSScreen::procRendPush(void* pointer) {

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

}

void FSScreen::procRendPop(void* pointer) {

	glPopMatrix();

}

void FSScreen::procRendScalation(void* pointer) {

	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;

	glScalef(x ,y ,z);

}

void FSScreen::procRendColor(void* pointer) {

	SRenderColor* n = (SRenderColor*) pointer;

	GLfloat red = n->red;
	GLfloat green = n->green;
	GLfloat blue = n->blue;
	GLfloat alpha = n->alpha;

	delete n;

	glColor4f(red,green,blue,alpha);

}

int FSScreen::beginRenderMode(Uint32 flags) {
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

int FSScreen::endRenderMode(Uint32 flags) {
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
	if (!m_SDL_Surface) {
		FSLibrary::I().Error("Video context not inicialized");
		return FRACASO;
	}

	SDL_FreeSurface(m_SDL_Surface);
	m_SDL_Surface=NULL;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	return EXITO;
}

Uint8 FSScreen::getBpp() {
	return m_Bpp;
}

int FSScreen::changeScreen(int width, int height, int bpp, float scalex, float scaley, bool fullscreen) {
	if (!m_SDL_Surface) {
		FSLibrary::I().Error("Video context not inicialized");
		return FRACASO;
	}
	

	clear();
	
	GraphicResources info;

	saveResources(info);

	quit();

	if ( start (width,height,bpp,scalex, scaley,fullscreen,m_Doublebuff) == FRACASO)
		return FRACASO;

	reloadResources(info);

	return EXITO;
	
}

int FSScreen::ToggleFullscreen() {

	if (!m_SDL_Surface) {
		FSLibrary::I().Error("Video context not inicialized");
		return FRACASO;
	}

	clear();

	GraphicResources info;

	saveResources(info);

	quit();

	m_FullScreen = !m_FullScreen;

	if ( start (m_Width,m_Height,m_Bpp,m_FullScreen,m_Doublebuff) == FRACASO )
		return FRACASO;	

	reloadResources(info);

	return EXITO;

}

int FSScreen::setDoublebuffer(bool doublebuff) {

	if (!m_SDL_Surface) {
		FSLibrary::I().Error("Video context not inicialized");
		return FRACASO;
	}

	if (doublebuff!=m_Doublebuff) {
		clear();

		GraphicResources info;

		saveResources(info);

		quit();

		if ( start (m_Width,m_Height,m_Bpp,m_FullScreen,doublebuff) == FRACASO)
			return FRACASO;

		reloadResources(info);
	}

	return EXITO;
}

void FSScreen::deleteResources() {
	
	for (list<FSSpriteset*>::iterator it = spritesetToDelete.begin(), jt = spritesetToDelete.end() ; it!=jt;++it)
		delete (*it);

	spritesetToDelete.clear();
	
	for (list<FSSprite*>::iterator it = spriteToDelete.begin(), jt = spriteToDelete.end() ; it!=jt;++it)
		delete (*it);

	spriteToDelete.clear();

	for (list<FSCanvas*>::iterator it = imageToDelete.begin(), jt = imageToDelete.end() ; it!=jt;++it)
		delete (*it);

	imageToDelete.clear();
}
