#include "FSControlOutputText.h"
#include "FSLibrary.h"
#include "FSparserXML.h"
#include "FSScreen.h"

CControlOutputText* CControlOutputText::singleton=NULL;

CControlOutputText::CControlOutputText() : admin((CEngine*)0xFFFFFFFF), data(NULL), fontSize(20) {
	singleton = NULL;

	if (TTF_Init()==-1) {
		CLibrary::Error("Failed to Init SDL_ttf:",TE_SDL_MSG);
		return;
	}

	atexit(TTF_Quit);

	if (singleton) {
		CLibrary::Error("COutputText (Write) ya estaba creado.",TE_controlViolation);
		return;
	}

	singleton = this;

	setAdmin(NULL);
}

CControlOutputText::~CControlOutputText() {
	clear();

	if (session.find(NULL)!=session.end()) 
		delete session[NULL];

	session.clear();
}

CEngine* CControlOutputText::setAdmin(CEngine* newAdmin) {

	CEngine* ret = admin;

#ifdef TEXT_OPERATIVE

	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return ret;
	}

	if (newAdmin != ret) {

		SData* thisData = session[newAdmin];
		if (!thisData) {
			thisData = session[newAdmin] = new SData;
			thisData->fgcolor.b=255;
			thisData->fgcolor.g=255;
			thisData->fgcolor.r=255;
		}
		data = thisData;

		admin = newAdmin;
	}
#endif
	return ret;
}

int CControlOutputText::setfontSize(int newSize) {
	int ret = fontSize;
	fontSize = newSize;
	return ret;
}

int CControlOutputText::searchFont(const char* name, int withSize) {
	int aux = setfontSize(withSize);
	int ret = searchFont(name);
	setfontSize(aux);
	return ret;
}

int CControlOutputText::searchFont(const char* name) {
#ifdef TEXT_OPERATIVE

	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	for (map<int,SFont*>::iterator it = Fonts.begin();it!=Fonts.end();++it) {
		if (strcmp(it->second->cadena.c_str(),name)==0 && it->second->size==fontSize) {
			return it->first;
		}
	}
#endif
	return FRACASO;
}

int CControlOutputText::searchFont(TTF_Font* fnt) {
#ifdef TEXT_OPERATIVE

	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	for (map<int,SFont*>::iterator it = Fonts.begin();it!=Fonts.end();++it) {
		if (it->second->fuente == fnt) {
			return it->first;
		}
	}
#endif
	return FRACASO;
}

int CControlOutputText::searchFont(int idtext) {
#ifdef TEXT_OPERATIVE

	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	TTF_Font* ret = NULL;

	for (map<CEngine*,SData*>::iterator kt=session.begin(),lt =session.end();kt!=lt && ret==NULL;++kt) {
		if (kt->second->Texts.find(idtext) != kt->second->Texts.end()) {
			SText* t = kt->second->Texts[idtext];

			switch (t->Type()) {
				case TT_LINE : ret = t->Line->fuente->fuente;
				case TT_BOX : ret = t->Box->fuente->fuente;
			} 
		}

	}

	if (ret)
		return searchFont(ret);
#endif
	return FRACASO;
}

int CControlOutputText::loadFont(const char* fuente,int withSize) {
	int aux = setfontSize(withSize);
	int ret = loadFont(fuente);
	setfontSize(aux);
	return ret;
}

int CControlOutputText::loadFont(const char* fuente) {

	string s(fuente);
	SFont* font_ttf;
	int ret= searchFont(fuente);
#ifdef TEXT_OPERATIVE

	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	if (ret < 0) {

		if (!lastIndexFontAdded.empty()) {
			ret=lastIndexFontAdded.back();
			lastIndexFontAdded.pop_back();
		} else {
			for (int i=0 ; ret < 0 ; i++) {
				if (Fonts.find(i)==Fonts.end()) {
					ret = i;
					break;
				}
			}
		}
		font_ttf = Fonts[ret] = new SFont;
		font_ttf->fuente=TTF_OpenFont(resource((s+".ttf").c_str()).c_str(),fontSize);
		if (font_ttf->fuente==NULL) {
			CLibrary::Error("No se ha cargado la fuente: "+s+".ttf  ",TE_fileExists);
			delete font_ttf;
			lastIndexFontAdded.push_back(ret);
			return FRACASO;
		}
		font_ttf->cadena=s;
		font_ttf->size=fontSize;
	} else {
		font_ttf=Fonts[ret];
	}

	++countFonts[font_ttf];
#endif

	return ret;

}

int CControlOutputText::unloadFont(const char* fuente,int withSize) {
	return unloadFont(searchFont(fuente,withSize));
}


int CControlOutputText::unloadFont(const char* fuente) {
	return unloadFont(searchFont(fuente));
}

int CControlOutputText::unloadFont(int fuente) {
#ifdef TEXT_OPERATIVE
	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	if (Fonts.find(fuente)==Fonts.end()) {
		CLibrary::Error("No existe la Fuente que se pretende eliminar.",TE_controlViolation);
		return FRACASO;
	}

	SFont* f = Fonts[fuente];
	int c = --countFonts[f];
	if (c < 1) {
		if (c==0) {
			TTF_CloseFont(f->fuente);
			map<Uint16,CImage*>& chars =f->render;
			while (!chars.empty()) {
				map<Uint16,CImage*>::iterator jt = chars.begin();
				CScreen::imageToDelete.push_back(jt->second); // delete jt->second;
				chars.erase(jt);
			}
			delete f;
			Fonts.erase(Fonts.find(fuente));
			countFonts.erase(countFonts.find(f));
			lastIndexFontAdded.push_back(fuente);
		} else {
			CLibrary::Error("Cantidad de Fuente violada.",TE_controlViolation);
			return FRACASO;
		}
	}

#endif
	return EXITO;
}
/*
CColor CControlOutputText::setColor(int r,int g,int b) {
#ifdef TEXT_OPERATIVE
	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	CColor ret(data->fgcolor.r,data->fgcolor.g,data->fgcolor.b);
	data->fgcolor.r=r;
	data->fgcolor.g=g;
	data->fgcolor.b=b;
	return ret;
#else
	return CColor(r,g,b);
#endif
}

CColor CControlOutputText::setColor(CColor& color) {
#ifdef TEXT_OPERATIVE
	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	CColor ret(data->fgcolor.r,data->fgcolor.g,data->fgcolor.b);
	data->fgcolor.r=color.getR();
	data->fgcolor.g=color.getG();
	data->fgcolor.b=color.getB();
	return ret;
#else
	return color;
#endif
}
*/
int CControlOutputText::line(int fuente, int x,int y, const char* text,...) {
	int ret = FRACASO;
#ifdef TEXT_OPERATIVE

	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (Fonts.find(fuente) != Fonts.end()) {
		va_list lista;
		char buffer [1024];

		va_start (lista, text);
		vsprintf (buffer, text, lista);

		va_end (lista);

		if (!data->lastIndexTextAdded.empty()) {
			ret = data->lastIndexTextAdded.back();
			data->lastIndexTextAdded.pop_back();
		} else {
			for (int i=0 ; ret < 0 ; i++) {
				if (data->Texts.find(i)==data->Texts.end()) {
					ret = i;
					break;
				}
			}
		}

		

		SText* t = data->Texts[ret] = new SText;
		t->Line->fuente = Fonts[fuente];

		string allText(buffer);

		float currentX = x;
		float currentY = y + (float)TTF_FontAscent(t->Line->fuente->fuente) -3;

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

			TTF_GlyphMetrics(t->Line->fuente->fuente,newChar,&minx,NULL,NULL,&maxy,&advance);

			if (newChar == '\n') {
				currentX = x;
				currentY += (float)TTF_FontLineSkip(t->Line->fuente->fuente);
			} else {

				SChar newT;

				newT.p = new CFloatPoint(currentX+(float)minx,currentY-(float)maxy);
				currentX += (float)advance;

				if (t->Line->fuente->render.find(newChar)==t->Line->fuente->render.end()) {
					SDL_Color fg;
					fg.b=0xFF;
					fg.g=0xFF;
					fg.r=0xFF;
					t->Line->fuente->render[newChar] = new CImage(CImage::toSCanvas(TTF_RenderGlyph_Blended(t->Line->fuente->fuente,newChar,fg)));
				}

				newT.glyph=newChar;

				t->Line->letra.push_back(newT);
			}

		}

	}

#endif
	return ret;

}

int CControlOutputText::erase(int text,bool nextframe) {
#ifdef TEXT_OPERATIVE

	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (data->Texts.find(text)!=data->Texts.end()) {
		
		if (nextframe) {
			
			data->deleteTextBuffer.push_back(text);

		} else {

			SText* t = data->Texts[text];

			if (t->Type() == TT_LINE && t->fx) {
				delete t->fx;
				t->fx = NULL;
			}

			delete t;
			data->lastIndexTextAdded.push_back(text);
			data->Texts.erase(data->Texts.find(text));
			

		}

	}	else if (text==ALL_TEXT) {

			while (!data->Texts.empty()) {
				map<int,SText*>::iterator it=data->Texts.begin();

				if (it->second->Type() == TT_LINE && it->second->fx) {
					delete it->second->fx;
					it->second->fx=NULL;
				}

				delete it->second;
				data->Texts.erase(it);				
			}

		while (!data->lastIndexTextAdded.empty())
			data->lastIndexTextAdded.pop_back();
	}

#endif
	return EXITO;
}

int CControlOutputText::inBox(const char* file, int index) {
	int ret = FRACASO;
#ifdef TEXT_OPERATIVE
	
	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (admin) {

		string s(file);
		s += ".xml";

		TiXmlDocument xmldoc(resource(s.c_str()).c_str());
		if (!xmldoc.LoadFile()) {	 CLibrary::Error(file,TE_fileExists); }	// Cargamos la biblioteca de textos.

		TiXmlElement* parent = xmldoc.FirstChildElement("TextList");

		if (!parent) {	 
			CLibrary::Error(file,TE_fileExists); 
			return FRACASO;
		}	

		TiXmlElement* textNode = parent->FirstChildElement("global");
		const char* gfont;
		int  gsize;

		if (textNode && textNode->Attribute("font") && textNode->Attribute("size")) {
			gfont = textNode->Attribute("font");
			gsize = textNode->QueryIntAttribute("size",&gsize);
		} else {
			if (Fonts.empty()) {
				CLibrary::Error("No hay fuente especificada");
				return FRACASO;
			}
			gfont = Fonts[0]->cadena.c_str();
			gsize = Fonts[0]->size;
		}

		textNode = parent->FirstChildElement("text");

		for (int i=0; (i < index) && textNode;i++)
			textNode = textNode->NextSiblingElement();

		if (textNode && textNode->Attribute("msg") && textNode->Attribute("x") && textNode->Attribute("y") && textNode->Attribute("w")) {

			int x,y,w;
			SFont* ttf_fnt;
			int next = -1;

			textNode->QueryIntAttribute("x",&x);
			textNode->QueryIntAttribute("y",&y);
			textNode->QueryIntAttribute("w",&w);

			if (textNode->Attribute("font") && textNode->Attribute("size")) {
				int fsize; 
				textNode->QueryIntAttribute("size",&fsize);
				ttf_fnt = Fonts[loadFont("font",fsize)];	
			} else {
				ttf_fnt = Fonts[loadFont(gfont,gsize)];
			}

			if (textNode->Attribute("next"))
				textNode->QueryIntAttribute("next",&next);

			if (!data->lastIndexTextAdded.empty()) {
				ret=data->lastIndexTextAdded.back();
				data->lastIndexTextAdded.pop_back();
			} else {
				for (int i=0 ; ret < 0 ; i++) {
					if (data->Texts.find(i)==data->Texts.end()) {
						ret = i;
						break;
					}
				}
			}

			data->Texts[ret] = new SText(file,textNode->Attribute("msg"),x,y,w,ttf_fnt,next);

			//data->Boxs.push_back(new CTextBox(file,textNode->Attribute("msg"),x,y,w,ttf_fnt,next));

		} else {
			CLibrary::Error("Texto no encontrado");
		}
	} else {
		CLibrary::Error("Los cuadros de texto deben crearse bajo el dominio de un manejador de eventos");
	}
#endif
	return ret;
}


int CControlOutputText::color(int text,float red, float green, float blue, float alpha, TypeColorTBox boxflags, bool persistent) {
#ifdef TEXT_OPERATIVE
	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (data->Texts.find(text)!=data->Texts.end()) {
	
			SText* t = data->Texts[text];

			SEffectText* fx = new SEffectText;

			fx->alpha = alpha;
			fx->blue = blue;
			fx->red = red;
			fx->green = green;
			fx->boxflags = boxflags;
			fx->persistent = persistent;

			if (t->Type() == TT_BOX) {
				if (t->Box->fx)
					delete t->Box->fx;
				t->Box->fx = fx;
			} else {
				if (t->fx)
					delete t->fx;
				t->fx = fx;
			}
			
	} else {

		CLibrary::Error("Text not found");
		return FRACASO;

	}
#endif

	return EXITO;

}

int CControlOutputText::color(int text,CColor* col, float alpha, TypeColorTBox boxflags, bool persistent) {

	return color(text,((float)col->getR())/255.0,((float)col->getG())/255.0,((float)col->getB())/255.0,alpha,boxflags,persistent);

}

int CControlOutputText::locateRenderScene ( float posx, float posy, float width, float height, float zoom) {

	if ( width == 0.0 || height == 0.0) {
		CLibrary::Error("Width\Height invalid value");
		return FRACASO;
	}

	this->posx = posx;
	this->posy = posy;
	this->width = width;
	this->height = height;
	this->zoom = zoom;

	return EXITO;
}

int CControlOutputText::render() {
#ifdef TEXT_OPERATIVE

	if (!singleton) {
		CLibrary::Error("SDL_ttf no inicializado, ver errores registrados al inicio.",TE_SDL_NOMSG);
		return FRACASO;
	}

	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());


	if ( width == 0.0 || height == 0.0) 
		CScreen::locateRenderScene(0,0,CScreen::getWidth(),CScreen::getHeight(),0); //
	else
		CScreen::locateRenderScene(posx,posy,width,height,zoom);


	map<int,SText*> deleteText;

	for (map<int,SText*>::iterator it=data->Texts.begin(),kt=data->Texts.end();it!=kt;++it) {

		SEffectText* fx = it->second->fx;

		if (it->second->Type() == TT_LINE) {
			SLineText* l = it->second->Line;
			for (list<SChar>::iterator jt=l->letra.begin(),ht=l->letra.end();jt!=ht;++jt) {
				if (fx) {
					l->fuente->render[jt->glyph]->color(fx->red,fx->green,fx->blue,fx->alpha);
				}
				
				l->fuente->render[jt->glyph]->put(*jt->p);
			}

			if (fx && !fx->persistent) {
				delete fx;
				it->second->fx = NULL;
			}
		}	else	if (it->second->Type() == TT_BOX) {
			CTextBox* b = it->second->Box;
			if (b->update()==-1) 
				deleteText[it->first]=it->second;
		}
	}

	for (map<int,SText*>::iterator it=deleteText.begin(),kt=deleteText.end();it!=kt;++it) {

		int aux = it->second->Box->finish();
		if (aux != -1) {
			data->Texts[it->first]=data->Texts[aux];
			data->Texts[aux]=it->second;
		} else {
			aux = it->first;
		}

		erase(aux);

	}

	if (session.find(NULL)!=session.end()) {
		for (map<int,SText*>::iterator it=session.begin()->second->Texts.begin(),kt=session.begin()->second->Texts.end();it!=kt;++it) {
			if (it->second->Type() == TT_LINE && it->second->Line) {
				SLineText* l = it->second->Line;
				SEffectText* fx = it->second->fx;
				for (list<SChar>::iterator jt=l->letra.begin(),ht=l->letra.end();jt!=ht;++jt)  {
					if (fx) 
						l->fuente->render[jt->glyph]->color(fx->red,fx->green,fx->blue,fx->alpha);
					
					l->fuente->render[jt->glyph]->put(*jt->p);
				}

				if (fx) {
					delete fx;
					it->second->fx = NULL;
				}
			}	
		}
	}

	while (!data->deleteTextBuffer.empty()) {
		erase(data->deleteTextBuffer.back());
		data->deleteTextBuffer.pop_back();
	}
#endif

	return EXITO;
}

void CControlOutputText::clear() {
#ifdef TEXT_OPERATIVE
	while (!session.empty()) {
		map<CEngine*,SData*>::iterator it= session.begin();
		setAdmin(it->first);

		while (!data->Texts.empty()) {
			map<int,SText*>::iterator it=data->Texts.begin();
			delete it->second;
			data->Texts.erase(it);				
		}

		while (!data->lastIndexTextAdded.empty())
			data->lastIndexTextAdded.pop_back();

		delete data;
		session.erase(session.find(admin));
	}

	admin=(CEngine*)0xFFFFFFFF;
	data=NULL;

	setAdmin(NULL);

	while (!Fonts.empty()) {
		map<int,SFont*>::iterator it = Fonts.begin();
		TTF_CloseFont(it->second->fuente);
		map<Uint16,CImage*>& chars = it->second->render;
		while (!chars.empty()) {
			map<Uint16,CImage*>::iterator jt = chars.begin();
			CScreen::imageToDelete.push_back(jt->second); // delete jt->second;
			chars.erase(jt);
		}
		delete it->second;
		Fonts.erase(it);
	}
	while (!countFonts.empty()) {
		map<SFont*,int>::iterator it = countFonts.begin();
		countFonts.erase(it);
	}
	while (!lastIndexFontAdded.empty())
		lastIndexFontAdded.pop_back();
#endif
}

CControlOutputText Write;
