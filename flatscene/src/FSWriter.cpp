#include "FSWriterImpl.h"
#include "FSLibrary.h"
#include "FSparserXML.h"
#include "FSScreen.h"

FSEngine* FSWriter::WriterImpl::setAdmin(FSEngine* newAdmin) {

    FSEngine* ret = admin;

#ifdef TEXT_OPERATIVE

    if (newAdmin != ret) {

        WriterImpl::SData* thisData = session[newAdmin];
        if (!thisData) {
            thisData = session[newAdmin] = new WriterImpl::SData;
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

FSWriter::FSWriter() : _impl(new WriterImpl) {
    _impl->admin = (FSEngine*) 0xFFFFFFFF;
    _impl->data = NULL;
    _impl->fontSize = 20;

	_impl->posx =
	_impl->posy =
	_impl->width =
	_impl->height =
	_impl->zoom = 0;

	if (TTF_Init()==-1) {
		FSLibrary::Error("Failed to Init SDL_ttf:",TE_SDL_MSG);
		return;
	}

	atexit(TTF_Quit);

	_impl->setAdmin(NULL);
}

FSWriter::~FSWriter() {
	clear();

	if (_impl->session.find(NULL)!=_impl->session.end())
		delete _impl->session[NULL];

	_impl->session.clear();
	delete _impl;
}

int FSWriter::setfontSize(int newSize) {
	int ret = _impl->fontSize;
	_impl->fontSize = newSize;
	return ret;
}

int FSWriter::searchFont(const char* name, int withSize) {
	int aux = setfontSize(withSize);
	int ret = searchFont(name);
	setfontSize(aux);
	return ret;
}

int FSWriter::searchFont(const char* name) {
#ifdef TEXT_OPERATIVE

	for (map<int,WriterImpl::SFont*>::iterator it = _impl->Fonts.begin();it!=_impl->Fonts.end();++it) {
		if (strcmp(it->second->cadena.c_str(),name)==0 && it->second->size==_impl->fontSize) {
			return it->first;
		}
	}
#endif
	return FRACASO;
}

int FSWriter::searchFont(TTF_Font* fnt) {
#ifdef TEXT_OPERATIVE

	for (map<int,WriterImpl::SFont*>::iterator it = _impl->Fonts.begin();it!=_impl->Fonts.end();++it) {
		if (it->second->fuente == fnt) {
			return it->first;
		}
	}
#endif
	return FRACASO;
}

int FSWriter::searchFont(int idtext) {
#ifdef TEXT_OPERATIVE

	TTF_Font* ret = NULL;

	for (map<FSEngine*,WriterImpl::SData*>::iterator kt=_impl->session.begin(),lt =_impl->session.end();kt!=lt && ret==NULL;++kt) {
		if (kt->second->Texts.find(idtext) != kt->second->Texts.end()) {
			WriterImpl::FSText* t = kt->second->Texts[idtext];

			switch (t->Type()) {
				case TT_LINE :
				    ret = t->Line->fuente->fuente;
				    break;
				case TT_BOX :
				    ret = t->Box->fuente->fuente;
				    break;
			} 
		}

	}

	if (ret)
		return searchFont(ret);
#endif
	return FRACASO;
}

int FSWriter::loadFont(const char* fuente,int withSize) {
	int aux = setfontSize(withSize);
	int ret = loadFont(fuente);
	setfontSize(aux);
	return ret;
}

int FSWriter::loadFont(const char* fuente) {

	string s(fuente);
	WriterImpl::SFont* font_ttf;
	int ret= searchFont(fuente);
#ifdef TEXT_OPERATIVE

	if (ret < 0) {

		if (!_impl->lastIndexFontAdded.empty()) {
			ret=_impl->lastIndexFontAdded.back();
			_impl->lastIndexFontAdded.pop_back();
		} else {
			for (int i=0 ; ret < 0 ; i++) {
				if (_impl->Fonts.find(i)==_impl->Fonts.end()) {
					ret = i;
					break;
				}
			}
		}
		font_ttf = _impl->Fonts[ret] = new WriterImpl::SFont;
		font_ttf->fuente=TTF_OpenFont((s+".ttf").c_str(),_impl->fontSize);
		if (font_ttf->fuente==NULL) {
			FSLibrary::Error("No se ha cargado la fuente: "+s+".ttf  ",TE_fileExists);
			delete font_ttf;
			_impl->lastIndexFontAdded.push_back(ret);
			return FRACASO;
		}
		font_ttf->cadena=s;
		font_ttf->size=_impl->fontSize;
	} else {
		font_ttf=_impl->Fonts[ret];
	}

	_impl->countFonts[font_ttf]++;
#endif

	return ret;

}

int FSWriter::unloadFont(const char* fuente,int withSize) {
	return unloadFont(searchFont(fuente,withSize));
}


int FSWriter::unloadFont(const char* fuente) {
	return unloadFont(searchFont(fuente));
}

int FSWriter::unloadFont(int fuente) {
#ifdef TEXT_OPERATIVE
	if (_impl->Fonts.find(fuente)==_impl->Fonts.end()) {
		FSLibrary::Error("No existe la Fuente que se pretende eliminar.",TE_controlViolation);
		return FRACASO;
	}

	WriterImpl::SFont* f = _impl->Fonts[fuente];
	int c = --(_impl->countFonts[f]);
	if (c < 1) {
		if (c==0) {
			TTF_CloseFont(f->fuente);
			map<Uint16,FSCanvas*>& chars =f->render;
			while (!chars.empty()) {
				map<Uint16,FSCanvas*>::iterator jt = chars.begin();
				FSScreen::imageToDelete.push_back(jt->second); // delete jt->second;
				chars.erase(jt);
			}
			delete f;
			_impl->Fonts.erase(_impl->Fonts.find(fuente));
			_impl->countFonts.erase(_impl->countFonts.find(f));
			_impl->lastIndexFontAdded.push_back(fuente);
		} else {
			FSLibrary::Error("Cantidad de Fuente violada.",TE_controlViolation);
			return FRACASO;
		}
	}

#endif
	return EXITO;
}
/*
CColor CWriter::setColor(int r,int g,int b) {
#ifdef TEXT_OPERATIVE
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

CColor CWriter::setColor(CColor& color) {
#ifdef TEXT_OPERATIVE
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
int FSWriter::line(int fuente, int x,int y, const char* text,...) {
	int ret = FRACASO;
#ifdef TEXT_OPERATIVE

	if (_impl->admin != FSLibrary::getActualEngine())
	    _impl->setAdmin(FSLibrary::getActualEngine());

	if (_impl->Fonts.find(fuente) != _impl->Fonts.end()) {
		va_list lista;
		char buffer [1024];

		va_start (lista, text);
		vsprintf (buffer, text, lista);

		va_end (lista);

		if (!_impl->data->lastIndexTextAdded.empty()) {
			ret = _impl->data->lastIndexTextAdded.back();
			_impl->data->lastIndexTextAdded.pop_back();
		} else {
			for (int i=0 ; ret < 0 ; i++) {
				if (_impl->data->Texts.find(i)==_impl->data->Texts.end()) {
					ret = i;
					break;
				}
			}
		}

		

		WriterImpl::FSText* t = _impl->data->Texts[ret] = new WriterImpl::FSText;
		t->Line->fuente = _impl->Fonts[fuente];

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

				WriterImpl::SChar newT;

				newT.p = new FSFloatPoint(currentX+(float)minx,currentY-(float)maxy);
				currentX += (float)advance;

				if (t->Line->fuente->render.find(newChar)==t->Line->fuente->render.end()) {
					SDL_Color fg;
					fg.b=0xFF;
					fg.g=0xFF;
					fg.r=0xFF;
					t->Line->fuente->render[newChar] = new FSCanvas(FSCanvas::toSCanvas(TTF_RenderGlyph_Blended(t->Line->fuente->fuente,newChar,fg)));
				}

				newT.glyph=newChar;

				t->Line->letra.push_back(newT);
			}

		}

	}

#endif
	return ret;

}

int FSWriter::erase(int text,bool nextframe) {
#ifdef TEXT_OPERATIVE

	if (_impl->admin != FSLibrary::getActualEngine())
	    _impl->setAdmin(FSLibrary::getActualEngine());

	if (_impl->data->Texts.find(text)!=_impl->data->Texts.end()) {
		
		if (nextframe) {
			
		    _impl->data->deleteTextBuffer.push_back(text);

		} else {

			WriterImpl::FSText* t = _impl->data->Texts[text];

			if (t->Type() == TT_LINE && t->fx) {
				delete t->fx;
				t->fx = NULL;
			}

			delete t;
			_impl->data->lastIndexTextAdded.push_back(text);
			_impl->data->Texts.erase(_impl->data->Texts.find(text));
			

		}

	}	else if (text==ALL_TEXT) {

			while (!_impl->data->Texts.empty()) {
				map<int,WriterImpl::FSText*>::iterator it=_impl->data->Texts.begin();

				if (it->second->Type() == TT_LINE && it->second->fx) {
					delete it->second->fx;
					it->second->fx=NULL;
				}

				delete it->second;
				_impl->data->Texts.erase(it);
			}

		while (!_impl->data->lastIndexTextAdded.empty())
		    _impl->data->lastIndexTextAdded.pop_back();
	}

#endif
	return EXITO;
}

int FSWriter::inBox(const char* file, int index) {
	int ret = FRACASO;
#ifdef TEXT_OPERATIVE
	
	if (_impl->admin != FSLibrary::getActualEngine())
	    _impl->setAdmin(FSLibrary::getActualEngine());

	if (_impl->admin) {

		string s(file);
		s += ".xml";

		TiXmlDocument xmldoc(s.c_str());
		if (!xmldoc.LoadFile()) {	 FSLibrary::Error(file,TE_fileExists); }	// Cargamos la biblioteca de textos.

		TiXmlElement* parent = xmldoc.FirstChildElement("TextList");

		if (!parent) {	 
			FSLibrary::Error(file,TE_fileExists); 
			return FRACASO;
		}	

		TiXmlElement* textNode = parent->FirstChildElement("global");
		const char* gfont;
		int  gsize;

		if (textNode && textNode->Attribute("font") && textNode->Attribute("size")) {
			gfont = textNode->Attribute("font");
			gsize = textNode->QueryIntAttribute("size",&gsize);
		} else {
			if (_impl->Fonts.empty()) {
				FSLibrary::Error("No hay fuente especificada");
				return FRACASO;
			}
			gfont = _impl->Fonts[0]->cadena.c_str();
			gsize = _impl->Fonts[0]->size;
		}

		textNode = parent->FirstChildElement("text");

		for (int i=0; (i < index) && textNode;i++)
			textNode = textNode->NextSiblingElement();

		if (textNode && textNode->Attribute("msg") && textNode->Attribute("x") && textNode->Attribute("y") && textNode->Attribute("w")) {

			int x,y,w;
			WriterImpl::SFont* ttf_fnt;
			int next = -1;

			textNode->QueryIntAttribute("x",&x);
			textNode->QueryIntAttribute("y",&y);
			textNode->QueryIntAttribute("w",&w);

			if (textNode->Attribute("font") && textNode->Attribute("size")) {
				int fsize; 
				textNode->QueryIntAttribute("size",&fsize);
				ttf_fnt = _impl->Fonts[loadFont("font",fsize)];
			} else {
				ttf_fnt = _impl->Fonts[loadFont(gfont,gsize)];
			}

			if (textNode->Attribute("next"))
				textNode->QueryIntAttribute("next",&next);

			if (!_impl->data->lastIndexTextAdded.empty()) {
				ret=_impl->data->lastIndexTextAdded.back();
				_impl->data->lastIndexTextAdded.pop_back();
			} else {
				for (int i=0 ; ret < 0 ; i++) {
					if (_impl->data->Texts.find(i)==_impl->data->Texts.end()) {
						ret = i;
						break;
					}
				}
			}

			_impl->data->Texts[ret] = new WriterImpl::FSText(file,textNode->Attribute("msg"),x,y,w,ttf_fnt,next);

			//data->Boxs.push_back(new CTextBox(file,textNode->Attribute("msg"),x,y,w,ttf_fnt,next));

		} else {
			FSLibrary::Error("Texto no encontrado");
		}
	} else {
		FSLibrary::Error("Los cuadros de texto deben crearse bajo el dominio de un manejador de eventos");
	}
#endif
	return ret;
}


int FSWriter::color(int text,float red, float green, float blue, float alpha, TypeColorTBox boxflags, bool persistent) {
#ifdef TEXT_OPERATIVE
	if (_impl->admin != FSLibrary::getActualEngine())
	    _impl->setAdmin(FSLibrary::getActualEngine());

	if (_impl->data->Texts.find(text)!=_impl->data->Texts.end()) {
	
			WriterImpl::FSText* t = _impl->data->Texts[text];

			WriterImpl::SEffectText* fx = new WriterImpl::SEffectText;

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

		FSLibrary::Error("Text not found");
		return FRACASO;

	}
#endif

	return EXITO;

}

int FSWriter::color(int text,FSColor* col, float alpha, TypeColorTBox boxflags, bool persistent) {

	return color(text,((float)col->getR())/255.0,((float)col->getG())/255.0,((float)col->getB())/255.0,alpha,boxflags,persistent);

}

int FSWriter::locateRenderScene ( float posx, float posy, float width, float height, float zoom) {

	if ( width == 0.0 || height == 0.0) {
		FSLibrary::Error("Width/Height invalid value");
		return FRACASO;
	}

	_impl->posx = posx;
	_impl->posy = posy;
	_impl->width = width;
	_impl->height = height;
	_impl->zoom = zoom;

	return EXITO;
}

int FSWriter::render() {
#ifdef TEXT_OPERATIVE

	if (_impl->admin != FSLibrary::getActualEngine())
	    _impl->setAdmin(FSLibrary::getActualEngine());


	if ( _impl->width == 0.0 || _impl->height == 0.0)
		FSScreen::locateRenderScene(0,0,FSScreen::getWidth(),FSScreen::getHeight(),0); //
	else
		FSScreen::locateRenderScene(_impl->posx,_impl->posy,_impl->width,_impl->height,_impl->zoom);


	map<int,WriterImpl::FSText*> deleteText;

	for (map<int,WriterImpl::FSText*>::iterator it=_impl->data->Texts.begin(),kt=_impl->data->Texts.end();it!=kt;++it) {

		WriterImpl::SEffectText* fx = it->second->fx;

		if (it->second->Type() == TT_LINE) {
			WriterImpl::SLineText* l = it->second->Line;
			for (list<WriterImpl::SChar>::iterator jt=l->letra.begin(),ht=l->letra.end();jt!=ht;++jt) {
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
			WriterImpl::FSTextBox* b = it->second->Box;
			if (b->update()==-1) 
				deleteText[it->first]=it->second;
		}
	}

	for (map<int,WriterImpl::FSText*>::iterator it=deleteText.begin(),kt=deleteText.end();it!=kt;++it) {

		int aux = it->second->Box->finish();
		if (aux != -1) {
		    _impl->data->Texts[it->first]=_impl->data->Texts[aux];
		    _impl->data->Texts[aux]=it->second;
		} else {
			aux = it->first;
		}

		erase(aux);

	}

	if (_impl->session.find(NULL)!=_impl->session.end()) {
		for (map<int,WriterImpl::FSText*>::iterator it=_impl->session.begin()->second->Texts.begin(),kt=_impl->session.begin()->second->Texts.end();it!=kt;++it) {
			if (it->second->Type() == TT_LINE && it->second->Line) {
				WriterImpl::SLineText* l = it->second->Line;
				WriterImpl::SEffectText* fx = it->second->fx;
				for (list<WriterImpl::SChar>::iterator jt=l->letra.begin(),ht=l->letra.end();jt!=ht;++jt)  {
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

	while (!_impl->data->deleteTextBuffer.empty()) {
		erase(_impl->data->deleteTextBuffer.back());
		_impl->data->deleteTextBuffer.pop_back();
	}
#endif

	return EXITO;
}

void FSWriter::clear() {
#ifdef TEXT_OPERATIVE
	while (!_impl->session.empty()) {
		map<FSEngine*,WriterImpl::SData*>::iterator it= _impl->session.begin();
		_impl->setAdmin(it->first);

		while (!_impl->data->Texts.empty()) {
			map<int,WriterImpl::FSText*>::iterator it=_impl->data->Texts.begin();
			delete it->second;
			_impl->data->Texts.erase(it);
		}

		while (!_impl->data->lastIndexTextAdded.empty())
		    _impl->data->lastIndexTextAdded.pop_back();

		delete _impl->data;
		_impl->session.erase(_impl->session.find(_impl->admin));
	}

	_impl->admin=(FSEngine*)0xFFFFFFFF;
	_impl->data=NULL;

	_impl->setAdmin(NULL);

	while (!_impl->Fonts.empty()) {
		map<int,WriterImpl::SFont*>::iterator it = _impl->Fonts.begin();
		TTF_CloseFont(it->second->fuente);
		map<Uint16,FSCanvas*>& chars = it->second->render;
		while (!chars.empty()) {
			map<Uint16,FSCanvas*>::iterator jt = chars.begin();
			FSScreen::imageToDelete.push_back(jt->second); // delete jt->second;
			chars.erase(jt);
		}
		delete it->second;
		_impl->Fonts.erase(it);
	}
	while (!_impl->countFonts.empty()) {
		map<WriterImpl::SFont*,int>::iterator it = _impl->countFonts.begin();
		_impl->countFonts.erase(it);
	}
	while (!_impl->lastIndexFontAdded.empty())
	    _impl->lastIndexFontAdded.pop_back();
#endif
}

#ifdef GLOBAL_SINGLETON_REFERENCES
FSWriter& Write = FSWriter::I();
#endif
