#include "TextBox.h"
#include "Time.h"
#include "SDL.h"

#include "SDL_rotozoom.h"
#include "Library.h"
#include "ControlOutputText.h"

#define MARGEN 10

CTextBox::CTextBox(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next) : 
file(file), fuente(ttf_fnt), next(next), upleft(x,y), fx(NULL),
timer(Chrono.getTick()), step(0), maxStep(0)	{

	string allText(text);
	Uint16 limite=xBox=Lim;

	float currentX = upleft.X()+MARGEN;
	float currentY = upleft.Y()+MARGEN-2;
	currentY += TTF_FontAscent(fuente->fuente)/2 ;

	col.r=255;
	col.b=255;
	col.g=255;

	size_t length = allText.length();

	while (length > 0 ) {

		Uint16 newChar = allText.c_str()[0];

		if ((newChar & 0x80)  && (length  > 1)) {

			if ((newChar & 0x20) && (length  > 2)) {

				allText =allText.substr(3,length);

			} else {


				newChar =  ((newChar & 0x1F) << 6) + ((allText.c_str()[1]) & 0x3F);

				allText =allText.substr(2,length);
			}

		} else {
			allText =allText.substr(1,length);
		}

		length = allText.length();		

		int minx,maxy,advance;

		if (TTF_GlyphMetrics(fuente->fuente,newChar,&minx,NULL,NULL,&maxy,&advance)== -1)
			CLibrary::Error("TTF_GlyphMetrics fallo.");

		if (newChar == ' ' ) {
			const char* caux = allText.c_str();
			float cuenta = ((float)advance) /((float)CScreen::getScaleX());

			for (int i=0;caux[i]!='\0' && caux[i]!=' ' && caux[i]!='\n';i++) {

				if (TTF_GlyphMetrics(fuente->fuente,caux[i],NULL,NULL,NULL,NULL,&minx) == -1)
					CLibrary::Error("TTF_GlyphMetrics fallo.");

				cuenta += ((float)minx) /((float)CScreen::getScaleX());

			}

			if ((currentX + cuenta - x)>= (limite - MARGEN)) {
				currentX = upleft.X() + MARGEN;
				currentY += ((float)TTF_FontLineSkip(fuente->fuente)) /((float)CScreen::getScaleY());
			} else {
				maxStep++;
				currentX += ((float)advance) /((float)CScreen::getScaleX());
				SChar newT;

				newT.p=NULL;

				charInDisplay.push_back(newT);
			}

		} else if (newChar == '\n') {
			currentX = upleft.X() + MARGEN;
			currentY += ((float)TTF_FontLineSkip(fuente->fuente)) /((float)CScreen::getScaleY());
		} else {
			maxStep++;

			SChar newT;

			currentX += ((float)minx) /((float)CScreen::getScaleX());
			currentY -= ((float)maxy) /((float)CScreen::getScaleY());
			newT.p = new CFloatPoint(currentX,currentY);
			currentX += ((float)advance) /((float)CScreen::getScaleX()) - ((float)minx) /((float)CScreen::getScaleX());
			currentY += ((float)maxy) /((float)CScreen::getScaleY());

			if (fuente->render.find(newChar)==fuente->render.end()) {
				fuente->render[newChar] = new CImage(CImage::toSCanvas(TTF_RenderGlyph_Blended(fuente->fuente,newChar,col)));
			}

			newT.glyph=newChar;

			charInDisplay.push_back(newT);
		}

	}

	 currentY -= (float)y;

	 currentY += (float)TTF_FontLineSkip(fuente->fuente) /((float)CScreen::getScaleY());

	 SDL_Surface* surf,* temp;

	 surf = SDL_CreateRGBSurface(0,limite,(int)currentY,32,0,0,255,0);
	if (!surf)	CLibrary::Error("No se ha creado bien la superficie para la TextBox.");
	temp = SDL_DisplayFormat(surf);
	if (!temp)	CLibrary::Error("No se ha creado bien la superficie para la TextBox.");
	SDL_FreeSurface(surf);
	SDL_FillRect(temp,NULL,SDL_MapRGB(temp->format,50,50,150));
	#if TEXT_BASE_SCALE > 1
    surf = rotozoomSurface(temp,0,CScreen::getScaleX() ,0);
    if (!surf)	CLibrary::Error("No se ha creado bien la superficie para la TextBox.");
    SDL_FreeSurface(temp);
    #else     
	surf=temp;
	#endif

	yBox = (Uint8) currentY;

	//SDL_SetAlpha(surf,0,255);


	box = new CImage(CImage::toSCanvas(surf));

}

CTextBox::~CTextBox() {
	deleteBox();

	if (fx)
		delete fx;
	fx = NULL;

	while (!charInDisplay.empty()) {
		list<SChar>::iterator it=charInDisplay.begin();
		if (it->p) {
			delete it->p;
		}
		it->p=NULL;
		charInDisplay.erase(it);
	}

	Write.unloadFont(Write.searchFont(fuente->fuente));
}

int CTextBox::update() {

	if (fx && ( fx->boxflags == TCTB_ALL || fx->boxflags == TCTB_BOX )) {
		box->color(fx->red,fx->green,fx->blue,fx->alpha);
	}

	box->put(upleft);

	if ((Chrono.getTick() > timer + 3) && (step <= maxStep)) {

		timer = Chrono.getTick();

		step++;

	} else if ( Chrono.getTick() > timer + 100) {
		while (!charInDisplay.empty()) {
			list<SChar>::iterator it=charInDisplay.begin();
			if (it->p) {
				delete it->p;
			}
			it->p=NULL;
			charInDisplay.erase(it);
		}
		return -1;
	}

	int i=0;
	for (list<SChar>::iterator it=charInDisplay.begin();it!=charInDisplay.end() && i<step;++it) {
		if (it->p) {
			if (fx && ( fx->boxflags == TCTB_ALL || fx->boxflags == TCTB_TEXT ))
				fuente->render[it->glyph]->color(fx->red,fx->green,fx->blue,fx->alpha);
			fuente->render[it->glyph]->put(*it->p);
		}
		i++;
	}

	if (fx && !fx->persistent) {
		delete fx;
		fx = NULL;
	}

	return 0;

}

void CTextBox::deleteBox() {
	if (box)
		CScreen::imageToDelete.push_back(box); // delete box;
	box=NULL;
}



void CTextBox::createBox() {
	if (box) {
		CLibrary::Error("Ya existe el fondo de la caja que se pretende crear.");
		return;
	}

	SDL_Surface* surf = SDL_CreateRGBSurface(0,xBox,yBox,32,0,0,255,0);
	if (!surf)	CLibrary::Error("No se ha creado bien la superficie para la TextBox.");
	SDL_Surface* temp = SDL_DisplayFormat(surf);
	if (!temp)	CLibrary::Error("No se ha creado bien la superficie para la TextBox.");
	SDL_FreeSurface(surf);
	SDL_FillRect(temp,NULL,SDL_MapRGB(temp->format,50,50,150));
	#if TEXT_BASE_SCALE > 1
    surf = rotozoomSurface(temp,0,TEXT_BASE_SCALE ,0);
    if (!surf)	CLibrary::Error("No se ha creado bien la superficie para la TextBox.");
    SDL_FreeSurface(temp);
    #else     
	surf=temp;
	#endif

	box = new CImage(CImage::toSCanvas(surf));
}

int CTextBox::finish() {
	int ret = -1;
	if (next!=-1) {

		ret = Write.inBox(file.c_str(),next);
		if (fx && fx->persistent) {
			Write.color(ret,fx->red,fx->green,fx->blue,fx->alpha,fx->boxflags,true);
		}
		if (ret == -1) {
			; // ODOT : Quitar el error de la cola.
		}

	}

	return ret;
}


SLineText::SLineText() {
}

SLineText::~SLineText() {
	for (list<SChar>::iterator it=letra.begin(),kt=letra.end();it!=kt;++it) {
		if (it->p) {
			delete it->p;
			it->p=NULL;
		}
	}
}
