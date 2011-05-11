#include "FSImage.h"
#include "FSLibrary.h"

CImage::CImage ( SCanvas pSurface ) 
{
	m_pSurface = pSurface;
}

CImage::~CImage ( ) 
{

	if (m_pSurface.sdl_surf) {
		SDL_FreeSurface(m_pSurface.sdl_surf);
	}

	if (m_pSurface.h != 0 || m_pSurface.w !=0)
		glDeleteTextures( 1, &(m_pSurface.tex) );

	clearSurface();
}

void CImage::initProcRenders() {

	procRenders[TR_ROTATION] = procRendRotation;
	procRenders[TR_TRANSLATION] = procRendTranslation;
	procRenders[TR_SCALATION] = procRendScalation;
	procRenders[TR_COLOR] = procRendColor;
	procRenders[TR_PUSHMATRIX] = procRendPush;
	procRenders[TR_POPMATRIX] = procRendPop;

}

void CImage::clearSurface ( ) 
{
	m_pSurface.bpp = m_pSurface.h = m_pSurface.h2 = m_pSurface.w = m_pSurface.w2 = m_pSurface.tex = 0;
	m_pSurface.sdl_surf = NULL;

}

Uint32 CImage::getPixel ( int x , int y ) 
{
	SDL_Surface* surface= m_pSurface.sdl_surf;
	if (surface && surface->w > x && surface->h > y) {
		Uint32 color = 0 ;
		int position = y * surface->pitch + surface->format->BytesPerPixel * x ;
		char* buffer = ( char* ) surface->pixels ;
		buffer += position ;
		memcpy ( &color , buffer , surface->format->BytesPerPixel ) ;
		return ( color ) ;
	} else {
		return 0;
	}
}

int CImage::getWidth () 
{
	return ( m_pSurface.w2 );
}

int CImage::getHeight () 
{
	return ( m_pSurface.h2 ) ;	
}

void CImage::put ( CFloatPoint& ptDst, Uint8 flags) 
{	

#ifdef MAINRENDERLOOP

	//PUSHMATRIX

	SToRender* em = new SToRender;

	em->type = TR_PUSHMATRIX;
	em->pointer = NULL;

	CScreen::graphicMaterial.push_back(em);

	//TRANSLATE

	SRenderTranscalation* c_init = new SRenderTranscalation();

	c_init->x = ptDst.X();
	c_init->y = ptDst.Y();
	c_init->z = 0.0;

	em = new SToRender();

	em->type = TR_TRANSLATION;
	em->pointer = (void*) c_init;

	CScreen::graphicMaterial.push_back(em);

	// USER DEFINED EFFECTS IN

	for (list<SToRender*>::iterator iri = initRenderList.begin(), ire = initRenderList.end();iri!=ire;++iri) {

		SToRender* irp = *iri;

		procRenders[irp->type](irp->pointer);

		delete irp;

	}

	initRenderList.clear();

	// PAINT FLOATCANVAS

	 em = new SToRender;

	em->type = TR_FLOATCANVAS;
	em->pointer = (void*) new SRenderFloatCanvas(m_pSurface,ptDst,flags);
	CScreen::graphicMaterial.push_back(em);

	for (list<SToRender*>::iterator eri = endRenderList.begin(), ere = endRenderList.end();eri!=ere;++eri) {

		SToRender* erp = *eri;

		procRenders[erp->type](erp->pointer);

		delete erp;

	}

	endRenderList.clear();

	// USER DEFINED EFFECTS OUT

	for (list<SToRender*>::iterator eri = endRenderList.begin(), ere = endRenderList.end();eri!=ere;++eri) {

		SToRender* erp = *eri;

		procRenders[erp->type](erp->pointer);

		delete erp;

	}

	endRenderList.clear();

	// POPMATRIX

	em = new SToRender;

	em->type = TR_POPMATRIX;
	em->pointer = NULL;

	CScreen::graphicMaterial.push_back(em);

#else

	if (m_pSurface.h != 0 || m_pSurface.w !=0 ) {
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, m_pSurface.tex); 

		glTranslatef((float)ptDst.X(),(float)ptDst.Y(),0);  

		float relW = (float)m_pSurface.w2/(float)m_pSurface.w;
		float relH = (float)m_pSurface.h2/(float)m_pSurface.h;

		#ifdef TEXTURE_BASE_SCALE
				glScalef((1.0/TEXTURE_BASE_SCALE ),(1.0/TEXTURE_BASE_SCALE ),0.0);
		#endif

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

		glPopMatrix();
	}



#endif

}

void CImage::put ( CPoint& ptDst, Uint8 flags) 
{	

#ifdef MAINRENDERLOOP

	//PUSHMATRIX

	SToRender* em = new SToRender;

	em->type = TR_PUSHMATRIX;
	em->pointer = NULL;

	CScreen::graphicMaterial.push_back(em);

	//TRANSLATE

	SRenderTranscalation* c_init = new SRenderTranscalation();

	c_init->x = ptDst.X();
	c_init->y = ptDst.Y();
	c_init->z = 0.0;

	em = new SToRender();

	em->type = TR_TRANSLATION;
	em->pointer = (void*) c_init;

	CScreen::graphicMaterial.push_back(em);

	// USER DEFINED EFFECTS IN

	for (list<SToRender*>::iterator iri = initRenderList.begin(), ire = initRenderList.end();iri!=ire;++iri) {

		SToRender* irp = *iri;

		procRenders[irp->type](irp->pointer);

		delete irp;

	}

	initRenderList.clear();

	// PAINT CANVAS

	em = new SToRender;

	em->type = TR_CANVAS;
	em->pointer = (void*) new SRenderCanvas(m_pSurface,ptDst,flags);
	CScreen::graphicMaterial.push_back(em);

	
	// USER DEFINED EFFECTS OUT

	for (list<SToRender*>::iterator eri = endRenderList.begin(), ere = endRenderList.end();eri!=ere;++eri) {

		SToRender* erp = *eri;

		procRenders[erp->type](erp->pointer);

		delete erp;

	}

	endRenderList.clear();

	// POPMATRIX

	em = new SToRender;

	em->type = TR_POPMATRIX;
	em->pointer = NULL;

	CScreen::graphicMaterial.push_back(em);


#else

	if (m_pSurface.h != 0 || m_pSurface.w !=0 ) {
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, m_pSurface.tex); 

		glTranslatef((float)ptDst.X(),(float)ptDst.Y(),0);  

		float relW = (float)m_pSurface.w2/(float)m_pSurface.w;
		float relH = (float)m_pSurface.h2/(float)m_pSurface.h;

		#ifdef TEXTURE_BASE_SCALE
				glScalef((1.0/TEXTURE_BASE_SCALE ),(1.0/TEXTURE_BASE_SCALE ),0.0);
		#endif

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

		glPopMatrix();
	}


#endif

}


SDL_Surface* CImage::scaleSurface( SDL_Surface* s_surf, int factor) {

	SDL_Surface* ret = NULL;

	if (s_surf == NULL || factor <= 1)
		return ret;

	Uint8 bpp = s_surf->format->BytesPerPixel;

	if (bpp == 4) // 32 bits

		ret = SDL_CreateRGBSurface(s_surf->flags,s_surf->w*factor,s_surf->h*factor,bpp*8,s_surf->format->Rmask,s_surf->format->Gmask,s_surf->format->Bmask,s_surf->format->Amask);

	else if (bpp == 1) // 8 bits

		ret = SDL_CreateRGBSurface(s_surf->flags,s_surf->w*factor,s_surf->h*factor,bpp*8,0,0,0,0);

	else {
		CLibrary::Error("depth mode not valid",TE_SDL_NOMSG);
		return ret;

	}

	char* newPixels = (char*) ret->pixels;
	char* oldPixels = (char*) s_surf->pixels;

	SDL_LockSurface(ret);
	SDL_LockSurface(s_surf);

	for (int y=0;y<s_surf->h;y++) {

		for (int x=0;x<s_surf->w;x++) {

			int pos_old = y * s_surf->pitch + x * bpp;

			for (int fx = 0 ; fx<factor;fx++) {
				for (int fy = 0 ; fy < factor ; fy++) {
					
					int pos_new = (y*factor + fy) * ret->pitch + (x*factor +fx) * bpp;

					for (int b=0;b<bpp;b++) {

						newPixels[pos_new+b]=oldPixels[pos_old+b];
					}
				}
			}

		}

	}

	SDL_UnlockSurface(s_surf);
	SDL_UnlockSurface(ret);

	return ret;
}

SCanvas CImage::toSCanvas( SDL_Surface* surface, Uint8 mode, GLint filter) {

	if (pow2(mode) != mode)
		CLibrary::Error("CCanvas::LoadIMG -> modo erroneo.");

	SCanvas pSurface;

	SDL_Surface* image;
	SDL_Rect area;

	if (surface == NULL) {
		CLibrary::Error("CCanvas::LoadIMG -> image Null.");
		pSurface.w = pSurface.h = pSurface.bpp = pSurface.w2 = pSurface.h2 = pSurface.tex = 0;
		pSurface.sdl_surf = NULL;
		return pSurface;
	} 
	
	
	pSurface.w2 = surface->w;
	pSurface.h2 = surface->h;
	pSurface.tex = 0;
    pSurface.w = pow2((Uint32)surface->w);
    pSurface.h = pow2((Uint32)surface->h);
	pSurface.bpp = surface->format->BytesPerPixel;

	if (mode == ONLY_TEXTURE || 	mode == WITH_SDL_SURFACE) {
		int saved_flags;
		int  saved_alpha;
	  
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			  image = SDL_CreateRGBSurface(
				  SDL_SWSURFACE, 
				  pSurface.w, 
				  pSurface.h, 
				  surface->format->BitsPerPixel,
				  0x000000ff, 
				  0x0000ff00, 
				  0x00ff0000, 
				  0xff000000);
		#else
			  image = SDL_CreateRGBSurface(
				  SDL_SWSURFACE,  
				  pSurface->w, 
				  pSurface->h, 
				  surface->format->BitsPerPixel,
				  0xff000000, 
				  0x00ff0000, 
				  0x0000ff00, 
				  0x000000ff);
		#endif
		if (image == NULL) {
			CLibrary::Error("CCanvas::LoadIMG -> image Null.");
			return pSurface;
		}

		
		saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
		saved_alpha = surface->format->alpha;
		if ( (saved_flags & SDL_SRCALPHA)   == SDL_SRCALPHA ) {
		  SDL_SetAlpha(surface, 0, 0);
		}

		area.x = 0;
		area.y = 0;
		area.w = surface->w;
		area.h = surface->h;

		SDL_BlitSurface(surface, &area, image, &area);
	    
		
		if ( (saved_flags & SDL_SRCALPHA)== SDL_SRCALPHA )  {
			SDL_SetAlpha(surface, saved_flags, saved_alpha);
		}

		// Have OpenGL generate a texture object handle for us
		glGenTextures(1, &pSurface.tex );

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D,pSurface.tex );
	    
		// Set the texture's stretching properties
		if (filter != GL_NEAREST || filter != GL_LINEAR)
			filter = GL_NEAREST;

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,filter);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,filter);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	    
		// Edit the texture object's surface data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 
								0,
								GL_RGBA, 
								pSurface.w, 
								pSurface.h, 
								0, 
								GL_RGBA, 
								GL_UNSIGNED_BYTE, 
								image->pixels );

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

		if ( image ) { 
			SDL_FreeSurface( image );
			image=NULL;
		}

	//if (mode & (WITH_SDL_SURFACE | ONLY_SDL_SURFACE))	{
	}	else	{
		image=SDL_CreateRGBSurface(SDL_SWSURFACE,surface->w,surface->h,12,0,0,0,0);
		SDL_Surface* temp = surface;
		surface=SDL_ConvertSurface(surface,image->format,SDL_SWSURFACE);
		SDL_FreeSurface ( image );
		SDL_FreeSurface ( temp );
	}

	if (mode & ONLY_SDL_SURFACE) {
		pSurface.h=0;
		pSurface.w=0;
	}

	if (mode & ONLY_TEXTURE) {
		SDL_FreeSurface( surface );
		surface=NULL;
	}

	pSurface.sdl_surf=surface;

	return pSurface;
}


Uint32 CImage::pow2 (Uint32 n) {
	Uint32 c=1;
	while (c < n) 
		c<<=1;

	return c;
}

int CImage::rotate(float angle, float x, float y, float z) {

	//SCALE

	SRenderRotation* c_init = new SRenderRotation();

	c_init->x = x;
	c_init->y = y;
	c_init->z = z;
	c_init->angle = angle;

	SToRender* r_init = new SToRender();

	r_init->type = TR_ROTATION;
	r_init->pointer = (void*) c_init;

	initRenderList.push_back(r_init);

	return EXITO;
}
int CImage::translate(float x, float y, float z) {

	//SCALE

	SRenderTranscalation* c_init = new SRenderTranscalation();

	c_init->x = x;
	c_init->y = y;
	c_init->z = z;

	SToRender* r_init = new SToRender();

	r_init->type = TR_TRANSLATION;
	r_init->pointer = (void*) c_init;

	initRenderList.push_back(r_init);

	return EXITO;
}
int CImage::scale(float x, float y, float z) {

	//SCALE

	SRenderTranscalation* c_init = new SRenderTranscalation();

	c_init->x = x;
	c_init->y = y;
	c_init->z = z;

	SToRender* r_init = new SToRender();

	r_init->type = TR_SCALATION;
	r_init->pointer = (void*) c_init;

	initRenderList.push_back(r_init);

	return EXITO;
}

int CImage::color(float red, float green, float blue, float alpha) {

	if (red > 1.0) red = 1.0;
	if (green > 1.0) green = 1.0;
	if (blue > 1.0) blue = 1.0;
	if (alpha > 1.0) alpha = 1.0;

	SRenderColor * c_init = new SRenderColor();

	c_init->red = red;
	c_init->green = green;
	c_init->blue = blue;
	c_init->alpha = alpha;

	SToRender* r_init = new SToRender();

	r_init->type = TR_COLOR;
	r_init->pointer = (void*) c_init;

	initRenderList.push_back(r_init);
	

	SRenderColor * c_fin = new SRenderColor();

	c_fin->red = CScreen::red;//2.0 - red;
	c_fin->green = CScreen::green;//2.0 - green;
	c_fin->blue = CScreen::blue;//2.0 - blue;
	c_fin->alpha =  CScreen::alpha;//2.0 - alpha;

	SToRender* r_fin = new SToRender();

	r_fin->type = TR_COLOR;
	r_fin->pointer = (void*) c_fin;

	endRenderList.push_front(r_fin);

	return EXITO;
}

int CImage::color(CColor* col, float alpha) {

	return color(((float)col->getR())/255.0,((float)col->getG())/255.0,((float)col->getB())/255.0,alpha);
}


void CImage::procRendPush(void* pointer) {
	CScreen::pushMatrix();
}

void CImage::procRendPop(void* pointer) {
	CScreen::popMatrix();
}

void CImage::procRendRotation(void* pointer) {

	SRenderRotation* n = (SRenderRotation*) pointer;

	GLint angle = n->angle;
	GLint x = n->x;
	GLint y = n->y;
	GLint z = n->z;

	delete n;

	CScreen::rotate(angle,x,y,z);

}

void CImage::procRendTranslation(void* pointer) {


	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;

	GLfloat z = n->z;

	delete n;

	CScreen::translate(x,y,z);

}

void CImage::procRendScalation(void* pointer) {

	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;

	CScreen::scale(x,y,z);


}

void CImage::procRendColor(void* pointer) {

	SRenderColor* n = (SRenderColor*) pointer;

	GLfloat red = n->red;
	GLfloat green = n->green;
	GLfloat blue = n->blue;
	GLfloat alpha = n->alpha;

	delete n;

	CScreen::color(red,green,blue,alpha);

}