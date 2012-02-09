#include "FSWriterImpl.h"
#include "FSTime.h"
#include "SDL.h"

#include "FSLibrary.h"

#define MARGEN 20

FSWriter::WriterImpl::FSTextBox::FSTextBox(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next) :
file(file), fuente(ttf_fnt), next(next), upleft(x,y), fx(NULL), box(NULL),
timer(Chrono.getTick()), step(0), maxStep(0)	{

	string allText(text);
	Uint16 limite=xBox=Lim;

	float currentX = upleft.x+MARGEN;
	float currentY = upleft.y+MARGEN-2;
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
			FSLibrary::I().Error("TTF_GlyphMetrics fallo.");

		if (newChar == ' ' ) {
			const char* caux = allText.c_str();
			float cuenta = (float)advance;

			for (int i=0;caux[i]!='\0' && caux[i]!=' ' && caux[i]!='\n';i++) {

				if (TTF_GlyphMetrics(fuente->fuente,caux[i],NULL,NULL,NULL,NULL,&minx) == -1)
					FSLibrary::I().Error("TTF_GlyphMetrics fallo.");

				cuenta += (float)minx;

			}

			if ((currentX + cuenta - x)>= (limite - MARGEN)) {
				currentX = upleft.x + MARGEN;
				currentY += (float)TTF_FontLineSkip(fuente->fuente);
			} else {
				maxStep++;
				currentX += (float)advance;
				SChar newT;

				newT.p=NULL;

				charInDisplay.push_back(newT);
			}
			
		} else if (newChar == '\n') {
			currentX = upleft.x + MARGEN;
			currentY += (float)TTF_FontLineSkip(fuente->fuente);
		} else {
			maxStep++;

			SChar newT;

			newT.p = new FSFloatPoint(currentX+(float)minx,currentY-(float)maxy);
			currentX += (float)advance;

			if (fuente->render.find(newChar)==fuente->render.end()) {
				fuente->render[newChar] = new FSCanvas(FSCanvas::toSCanvas(TTF_RenderGlyph_Blended(fuente->fuente,newChar,col)));
			}

			newT.glyph=newChar;

			charInDisplay.push_back(newT);
		}

	}

	 currentY -= (float)y;

	 currentY += (float)TTF_FontLineSkip(fuente->fuente);

	yBox = (Uint8) currentY;

	createBox();

}

FSWriter::WriterImpl::FSTextBox::~FSTextBox() {
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

int FSWriter::WriterImpl::FSTextBox::update() {

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

void FSWriter::WriterImpl::FSTextBox::deleteBox() {
	if (box)
		FSScreen::I().imageToDelete.push_back(box); // delete box;
	box=NULL;
}



void FSWriter::WriterImpl::FSTextBox::createBox() {
	if (box) {
		FSLibrary::I().Error("Ya existe el fondo de la caja que se pretende crear.");
		return;
	}

	SDL_Surface *surface, *aux_surf;

	aux_surf = SDL_CreateRGBSurface(0,xBox,yBox,FSScreen::I().getBpp(),0,0,255,0);
	if (!aux_surf)	FSLibrary::I().Error("No se ha creado bien la superficie para la TextBox.");
	surface = SDL_DisplayFormat(aux_surf);
	if (!surface)	FSLibrary::I().Error("No se ha creado bien la superficie para la TextBox.");
	SDL_FreeSurface(aux_surf);
	SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,50,50,150));

	box = new FSCanvas(FSCanvas::toSCanvas(surface));
}

int FSWriter::WriterImpl::FSTextBox::finish() {
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

FSWriter::WriterImpl::SLineText::~SLineText() {
	for (list<SChar>::iterator it=letra.begin(),kt=letra.end();it!=kt;++it) {
		if (it->p) {
			delete it->p;
			it->p=NULL;
		}
	}
}
