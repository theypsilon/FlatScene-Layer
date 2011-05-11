#include "FSScreen.h"
#include "FSLibrary.h"
#include "FSEngine.h"
#include "FSControlOutputText.h"

SDL_Surface* CScreen::m_SDL_Surface=NULL;

bool CScreen::m_FullScreen=false;
bool CScreen::m_Doublebuff=true;
bool CScreen::rendering=false;
float CScreen::m_maxZ=400.0;

TypeRendeProjection CScreen::trp=TRP_PERSPECTIVE;

int CScreen::m_Bpp=0;
int CScreen::m_Width=0;
int CScreen::m_Height=0;

float CScreen::alpha =1.0;
float CScreen::red =1.0;
float CScreen::green =1.0;
float CScreen::blue =1.0;

list<SToRender*> CScreen::graphicMaterial;

list<CSprite*> CScreen::spriteToDelete;
list<CSpriteset*> CScreen::spritesetToDelete;
list<CImage*> CScreen::imageToDelete;

map<TypeResource,void (*)(void*)> CScreen::procRenders;

int CScreen::start(int width, int height, int bpp, bool fullscreen, bool doublebuff)
{ 
	if (CLibrary::getLibrary() == NULL) {
		CLibrary::Error("Library not inicialized");
		return FRACASO;
	}
		
	if (m_SDL_Surface) {
		CLibrary::Error("Video ya inicializado, orden imposible 'start'\n"); 
		return FRACASO;
	}

	if (SDL_InitSubSystem(SDL_INIT_VIDEO)==-1) {
		CLibrary::Error("SDL_InitSubSystem(SDL_INIT_VIDEO) falla : ",TE_SDL_MSG);
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
		CLibrary::Error("SDL_SetVideoMode ( width , height , bpp, flags) falla : ",TE_SDL_MSG);
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

int CScreen::start(int width, int height, int bpp, float scalex, float scaley, bool fullscreen, bool doublebuff)
{ 
	if (CLibrary::getLibrary() == NULL) {
		CLibrary::Error("Library not inicialized");
		return FRACASO;
	}
		
	if (m_SDL_Surface) {
		CLibrary::Error("Video ya inicializado, orden imposible 'start'\n"); 
		return FRACASO;
	}

	if (SDL_InitSubSystem(SDL_INIT_VIDEO)==-1) {
		CLibrary::Error("SDL_InitSubSystem(SDL_INIT_VIDEO) falla : ",TE_SDL_MSG);
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
		CLibrary::Error("SDL_SetVideoMode ( width , height , bpp, flags) falla : ",TE_SDL_MSG);
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

void CScreen::initProcRenders() {

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

int CScreen::render ( ) 
{
	if (!m_SDL_Surface) {
		CLibrary::Error("Video context not inicialized");
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

int CScreen::clear ( ) 
{
	if (!m_SDL_Surface) {
		CLibrary::Error("Video context not inicialized");
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

}

float CScreen::getA() {
	return alpha;
}

float CScreen::getR() {
	return red;
}

float CScreen::getG() {
	return green;
}

float CScreen::getB() {
	return blue;
}

int CScreen::locateRenderScene(float posx, float posy, float width, float height,float zoom) {

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

int CScreen::rotate(float angle, float x, float y, float z) {


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

int CScreen::translate(float x, float y, float z) {

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

int CScreen::scale(float x, float y, float z) {

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

int CScreen::color(float red, float green, float blue, float alpha) {

#ifdef MAINRENDERLOOP

	SRenderColor* n = new SRenderColor;

	CScreen::red = n->red = red;
	CScreen::green = n->green = green;
	CScreen::blue = n->blue = blue;
	CScreen::alpha = n->alpha = alpha;

	SToRender* em = new SToRender;

	em->type = TR_COLOR;
	em->pointer = (void*) n;

	graphicMaterial.push_back(em);

#else

	glColor4f(red,green,blue,alpha);

#endif

	return EXITO;
}

int CScreen::color(CColor* col, float alpha) {

	return color(((float)col->getR())/255.0,((float)col->getG())/255.0,((float)col->getB())/255.0,alpha);

}

int CScreen::projectionMode(TypeRendeProjection trp, float zMax) {

	if (rendering) {
		CLibrary::Error("No se puede cambiar el modo de proyección mientras se está en fase de renderización");
		return FRACASO;
	}

	m_maxZ = zMax;
	CScreen::trp = trp;

	return EXITO;

}

int CScreen::pushMatrix() {

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

int CScreen::popMatrix() {

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



void CScreen::procRendCanvas(void* pointer) {

	SRenderCanvas* n = (SRenderCanvas*) pointer;

	SCanvas m_pSurface = n->canvas;
	Uint8 flags = n->flags;
	CPoint ptDst = n->ptDst;

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

void CScreen::procRendFloatCanvas(void* pointer) {

	SRenderFloatCanvas* n = (SRenderFloatCanvas*) pointer;

	SCanvas m_pSurface = (n->canvas);
	Uint8 flags = n->flags;
	CFloatPoint ptDst = n->ptDst;

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
void CScreen::procRendRotation(void* pointer) {

	SRenderRotation* n = (SRenderRotation*) pointer;

	GLfloat angle = n->angle;
	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;

	glRotatef(angle,x,y,z);

}

void CScreen::procRendTranslation(void* pointer) {


	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;
			
	glTranslatef(x,y,z);


}

void CScreen::procRendLocation(void* pointer) {

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

void CScreen::procRendPush(void* pointer) {

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

}

void CScreen::procRendPop(void* pointer) {

	glPopMatrix();

}

void CScreen::procRendScalation(void* pointer) {

	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;

	glScalef(x ,y ,z);

}

void CScreen::procRendColor(void* pointer) {

	SRenderColor* n = (SRenderColor*) pointer;

	GLfloat red = n->red;
	GLfloat green = n->green;
	GLfloat blue = n->blue;
	GLfloat alpha = n->alpha;

	delete n;

	glColor4f(red,green,blue,alpha);

}

int CScreen::beginRenderMode(Uint32 flags) {
	if (!m_SDL_Surface) {
		CLibrary::Error("Video context not inicialized");
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

int CScreen::endRenderMode(Uint32 flags) {
	if (!m_SDL_Surface) {
		CLibrary::Error("Video context not inicialized");
		return FRACASO;
	}

	if (flags & RENDER_TEXTURE_STANDARD) {
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);
	}

	return EXITO;
}


int CScreen::quit()
{
	if (!m_SDL_Surface) {
		CLibrary::Error("Video context not inicialized");
		return FRACASO;
	}

	SDL_FreeSurface(m_SDL_Surface);
	m_SDL_Surface=NULL;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	return EXITO;
}

Uint8 CScreen::getBpp() {
	return m_Bpp;
}

int CScreen::changeScreen(int width, int height, int bpp, float scalex, float scaley, bool fullscreen) {
	if (!m_SDL_Surface) {
		CLibrary::Error("Video context not inicialized");
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

int CScreen::ToggleFullscreen() {

	if (!m_SDL_Surface) {
		CLibrary::Error("Video context not inicialized");
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

int CScreen::setDoublebuffer(bool doublebuff) {

	if (!m_SDL_Surface) {
		CLibrary::Error("Video context not inicialized");
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

void CScreen::deleteResources() {
	
	for (list<CSpriteset*>::iterator it = spritesetToDelete.begin(), jt = spritesetToDelete.end() ; it!=jt;++it)
		delete (*it);

	spritesetToDelete.clear();
	
	for (list<CSprite*>::iterator it = spriteToDelete.begin(), jt = spriteToDelete.end() ; it!=jt;++it)
		delete (*it);

	spriteToDelete.clear();

	for (list<CImage*>::iterator it = imageToDelete.begin(), jt = imageToDelete.end() ; it!=jt;++it)
		delete (*it);

	imageToDelete.clear();
}

void CScreen::saveResources(GraphicResources &info) {
	
	for (SpritesetCollection::iterator it=CImg.set.begin(),kt=CImg.set.end();it!=kt;++it)	{
		SpritesetInfo aux;
		aux.number = it->first;
		aux.name =	it->second->getName();
		aux.mode = it->second->getMode();
		aux.times = CImg.getCount(it->first);
		info.push_back(aux);
	}

	CImg.clear();

	for (map<int,SFont*>::iterator it=Write.Fonts.begin();it!=Write.Fonts.end();++it) {
		map<Uint16,CImage*>& chars = it->second->render;
		for (map<Uint16,CImage*>::iterator jt=chars.begin();jt!=chars.end();++jt) {
			imageToDelete.push_back(jt->second); // delete
			jt->second = NULL;
		}
	}

	for (map<CEngine*,SData*>::iterator it=Write.session.begin();it!=Write.session.end();++it) {
		map<int,SText*>& auxBoxs = it->second->Texts;
		for (map<int,SText*>::iterator jt=auxBoxs.begin();jt!=auxBoxs.end();++jt) {
			if (jt->second->Type() == TT_BOX && jt->second->Box)
				jt->second->Box->deleteBox();
		}
	}

	deleteResources();

}

void CScreen::reloadResources(GraphicResources &info) {
	
	for (map<int,SFont*>::iterator it=Write.Fonts.begin();it!=Write.Fonts.end();++it) {
		map<Uint16,CImage*>& chars = it->second->render;
		for (map<Uint16,CImage*>::iterator jt=chars.begin();jt!=chars.end();++jt) {
			if (!jt->second) {
				jt->second = new CSprite(CImage::toSCanvas(TTF_RenderGlyph_Blended(it->second->fuente,jt->first,Write.data->fgcolor)));
			} else {
				return CLibrary::Error("No se puede recargar el recurso glyph porque no había sido descargado anteriormente.");
			}
		}
	}

	for (map<CEngine*,SData*>::iterator it=Write.session.begin();it!=Write.session.end();++it) {
		map<int,SText*>& auxBoxs = it->second->Texts;
		for (map<int,SText*>::iterator jt=auxBoxs.begin();jt!=auxBoxs.end();++jt) {
			if (jt->second->Type() == TT_BOX && jt->second->Box)
				jt->second->Box->createBox();
		}
	}

	int number = 0;
	int aux = -1;

	for (GraphicResources::iterator it=info.begin(),et=info.end();it!=et;++it) {
		number = CImg.add((*it).name.c_str(),(*it).mode);
		if (number != it->number) {
			CImg.set[it->number] = CImg.set[number];
			CImg.set.erase(number);
			CImg.lastIndexAdded.push(number);
		}
		CImg.count[CImg.get(it->number)]=it->times;
	}
}