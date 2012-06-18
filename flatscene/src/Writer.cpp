#include "WriterImpl.h"
#include "Library.h"
#include "parserXML.h"
#include "ScreenImpl.h"
#include "Exception.h"
#include "CanvasFactory.h"

#include <algorithm>

namespace FlatScene {

Engine* Writer::WriterImpl::setAdmin(const Engine *const constAdmin) {

    auto ret = admin;
    auto newAdmin = const_cast<Engine*>(constAdmin);

#ifdef TEXT_OPERATIVE

    if (newAdmin != ret) {

        WriterImpl::SData& thisData = session[newAdmin];
        data = std::addressof(thisData);

        admin = newAdmin;
    }
#endif
    return ret;
}

Writer::Writer() {
    _impl->admin = nullptr;
    _impl->data = nullptr;
    _impl->fontSize = 20;

    _impl->posx =
    _impl->posy =
    _impl->width =
    _impl->height =
    _impl->zoom = 0;

    if (TTF_Init()==-1) {
        Library::I().Error("Failed to Init SDL_ttf:",TE_SDL_MSG);
        return;
    }

    atexit(TTF_Quit);

    _impl->setAdmin(nullptr);
}

Writer::~Writer() {
    clear();
}

int Writer::setfontSize(int newSize) {
    int ret = _impl->fontSize;
    _impl->fontSize = newSize;
    return ret;
}

int Writer::searchFont(const char* name, int withSize) {
    int aux = setfontSize(withSize);
    int ret = searchFont(name);
    setfontSize(aux);
    return ret;
}

int Writer::searchFont(const char* name) {
#ifdef TEXT_OPERATIVE

    for (auto it = _impl->Fonts.begin();it!=_impl->Fonts.end();++it) {
        if (it->second.cadena == name && it->second.size==_impl->fontSize) {
            return it->first;
        }
    }
#endif
    return FRACASO;
}

int Writer::searchFont(TTF_Font* fnt) {
#ifdef TEXT_OPERATIVE

    for (auto it = _impl->Fonts.begin();it!=_impl->Fonts.end();++it) {
        if (it->second.fuente == fnt) {
            return it->first;
        }
    }
#endif
    return FRACASO;
}

int Writer::searchFont(int idtext) {
#ifdef TEXT_OPERATIVE

    TTF_Font* ret = nullptr;

    for (auto kt=_impl->session.begin(),lt =_impl->session.end();kt!=lt && ret==nullptr;++kt) {
        if (kt->second.Texts.find(idtext) != kt->second.Texts.end()) {
            ret = kt->second.Texts.at(idtext).Object->fuente->fuente;
        }

    }

    if (ret)
        return searchFont(ret);
#endif
    return FRACASO;
}

int Writer::loadFont(const char* fuente,int withSize) {
    int aux = setfontSize(withSize);
    int ret = loadFont(fuente);
    setfontSize(aux);
    return ret;
}

int Writer::loadFont(const char* fuente) {

    std::string s(fuente);
    int ret= searchFont(fuente);
#ifdef TEXT_OPERATIVE

    if (ret < 0) {
        if (!_impl->lastIndexFontAdded.empty()) {
            ret=_impl->lastIndexFontAdded.back();
            _impl->lastIndexFontAdded.pop_back();
        } else 
            for (int i=0 ; ret < 0 ; i++)
                if (_impl->Fonts.find(i)==_impl->Fonts.end()) 
                    ret = (i > std::numeric_limits<int>::max())? throw Exception("So many fonts",__LINE__) : i;
        _impl->Fonts.insert(std::pair<int,WriterImpl::SFont>(
            ret,
            std::move(WriterImpl::SFont(s,_impl->fontSize))
        ));
    } 

    auto font_ttf = std::addressof(_impl->Fonts.at(ret));
    _impl->countFonts[font_ttf]++;
#endif

    return ret;

}

int Writer::unloadFont(const char* fuente,int withSize) {
    return unloadFont(searchFont(fuente,withSize));
}


int Writer::unloadFont(const char* fuente) {
    return unloadFont(searchFont(fuente));
}

int Writer::unloadFont(int fuente) {
#ifdef TEXT_OPERATIVE
    if (_impl->Fonts.find(fuente)==_impl->Fonts.end()) {
        Library::I().Error("No existe la Fuente que se pretende eliminar.",TE_controlViolation);
        return FRACASO;
    }

    WriterImpl::SFont* f = std::addressof(_impl->Fonts.at(fuente));
    int c = --(_impl->countFonts[f]);
    if (c < 1) {
        if (c==0) {
            _impl->Fonts.erase(_impl->Fonts.find(fuente));
            _impl->countFonts.erase(_impl->countFonts.find(f));
            _impl->lastIndexFontAdded.push_back(fuente);
        } else {
            throw Exception("Cantidad de Fuente violada.",__LINE__);
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
#define _CRT_SECURE_NO_WARNINGS

int Writer::line(int fuente, int x,int y, const char* text,...) {
    int ret = FRACASO;
#ifdef TEXT_OPERATIVE

    if (_impl->admin != Library::I().getActualEngine())
        _impl->setAdmin(Library::I().getActualEngine());

    if (_impl->Fonts.find(fuente) != _impl->Fonts.end()) {
        va_list lista;
        char buffer [1024];

        va_start (lista, text);
#ifdef _MSC_VER
        vsprintf_s (buffer, text, lista);
#else
        vsprintf (buffer, text, lista);
#endif
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

        _impl->data->Texts.insert(std::make_pair(
            std::move(ret),
            std::move(WriterImpl::FSText())
        ));

        auto& line = static_cast<WriterImpl::SLineText&>(*_impl->data->Texts.at(ret).Object);
        line.fuente = std::addressof(_impl->Fonts.find(fuente)->second);

        std::string allText(buffer);

        Float currentX = (Float) x;
        Float currentY = (Float) y + (Float)TTF_FontAscent(line.fuente->fuente) -3;

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

            TTF_GlyphMetrics(line.fuente->fuente,newChar,&minx,nullptr,nullptr,&maxy,&advance);

            if (newChar == '\n') {
                currentX = (Float) x;
                currentY += (Float)TTF_FontLineSkip(line.fuente->fuente);
            } else {

                WriterImpl::SChar newT;

                newT.p.set(currentX+(Float)minx,currentY-(Float)maxy);
                currentX += (Float)advance;

                if (line.fuente->render.find(newChar)==line.fuente->render.end()) {
                    SDL_Color fg;
                    fg.b=0xFF;
                    fg.g=0xFF;
                    fg.r=0xFF;

                    Canvas&& spt = Canvas::createCanvas<Canvas>(TTF_RenderGlyph_Blended(line.fuente->fuente,newChar,fg));

                    line.fuente->render.insert(std::make_pair<Uint16,Canvas>(
                        std::move(newChar),
                        std::move(spt)
                    ));
                }

                newT.glyph=newChar;

                line.letra.push_back(newT);
            }

        }

    }

#endif
    return ret;

}

void Writer::lineOnce(int fuente, int x,int y, const char* text,...) {

    va_list lista;
    char buffer [1024];

    va_start (lista, text);
#ifdef _MSC_VER
    vsprintf_s (buffer, text, lista);
#else
    vsprintf (buffer, text, lista);
#endif
    va_end (lista);

    erase(line(fuente,x,y,buffer),true);
}

int Writer::erase(int text,bool nextframe) {
#ifdef TEXT_OPERATIVE

    if (_impl->admin != Library::I().getActualEngine())
        _impl->setAdmin(Library::I().getActualEngine());
    auto textIter = _impl->data->Texts.find(text);
    if (textIter!=_impl->data->Texts.end()) {

        if (nextframe) {

            _impl->data->deleteTextBuffer.push_back(text);

        } else {

            _impl->data->Texts.erase(textIter);
            _impl->data->lastIndexTextAdded.push_back(text);

        }

    }   else if (text==ALL_TEXT) {

        _impl->data->Texts.clear();
        _impl->data->lastIndexTextAdded.clear();

    }

#endif
    return EXITO;
}

int Writer::inBox(const char* file, int index) {
    int ret = FRACASO;
#ifdef TEXT_OPERATIVE
    
    if (_impl->admin != Library::I().getActualEngine())
        _impl->setAdmin(Library::I().getActualEngine());

    if (_impl->admin) {

        std::string s(file);
        s += ".xml";

        TiXmlDocument xmldoc(s.c_str());
        if (!xmldoc.LoadFile()) {    Library::I().Error(file,TE_fileExists); }    // Cargamos la biblioteca de textos.

        TiXmlElement* parent = xmldoc.FirstChildElement("TextList");

        if (!parent) {
            Library::I().Error(file,TE_fileExists);
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
                Library::I().Error("No hay fuente especificada");
                return FRACASO;
            }

            gfont = _impl->Fonts.at(0).cadena.c_str();
            gsize = _impl->Fonts.at(0).size;
        }

        textNode = parent->FirstChildElement("text");

        for (int i=0; (i < index) && textNode;i++)
            textNode = textNode->NextSiblingElement();

        if (textNode && textNode->Attribute("msg") && textNode->Attribute("x") && textNode->Attribute("y") && textNode->Attribute("w")) {

            int x,y,w;

            WriterImpl::SFont* ttf_fnt = nullptr;
            int next = -1;

            textNode->QueryIntAttribute("x",&x);
            textNode->QueryIntAttribute("y",&y);
            textNode->QueryIntAttribute("w",&w);

            if (textNode->Attribute("font") && textNode->Attribute("size")) {
                int fsize;
                textNode->QueryIntAttribute("size",&fsize);
                ttf_fnt = std::addressof(_impl->Fonts.at(loadFont("font",fsize)));
            } else {
                ttf_fnt = std::addressof(_impl->Fonts.at(loadFont(gfont,gsize)));
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

            _impl->data->Texts.insert(std::make_pair(
                std::move(ret),
                std::move(WriterImpl::FSText(file,textNode->Attribute("msg"),x,y,w,ttf_fnt,next))
             ));

            //data->Boxs.push_back(new CTextBox(file,textNode->Attribute("msg"),x,y,w,ttf_fnt,next));

        } else {
            throw Exception("Text not found",__LINE__);
        }
    } else {
        throw Exception("No admin detected for handling this textbox",__LINE__);
    }
#endif
    return ret;
}


int Writer::color(int text,Float red, Float green, Float blue, Float alpha, TypeColorTBox boxflags, bool persistent) {
#ifdef TEXT_OPERATIVE
    if (_impl->admin != Library::I().getActualEngine())
        _impl->setAdmin(Library::I().getActualEngine());

    if (_impl->data->Texts.find(text)!=_impl->data->Texts.end()) {
    
            WriterImpl::FSText& t = _impl->data->Texts.at(text);

            if (dynamic_cast<WriterImpl::FSTextBox*>(t.Object.get())) {
                ;
            }
            
            auto& fx = dynamic_cast<WriterImpl::FSTextBox*>(t.Object.get())
                ? dynamic_cast<WriterImpl::FSTextBox*>(t.Object.get())->fx 
                : t.fx;

            fx.reset(new WriterImpl::SEffectText);

            fx->alpha = alpha;
            fx->blue = blue;
            fx->red = red;
            fx->green = green;
            fx->boxflags = boxflags;
            fx->persistent = persistent;

            

    } else {

        throw Exception("Text not found",__LINE__);

    }
#endif

    return EXITO;

}

int Writer::color(int text,Color* col, Float alpha, TypeColorTBox boxflags, bool persistent) {

    return color(text,((Float)col->getR())/255.0f,((Float)col->getG())/255.0f,((Float)col->getB())/255.0f,alpha,boxflags,persistent);

}

int Writer::locateRenderScene ( Float posx, Float posy, Float width, Float height, Float zoom) {

    if ( width == 0.0 || height == 0.0) {
        Library::I().Error("Width/Height invalid value");
        return FRACASO;
    }

    _impl->posx = posx;
    _impl->posy = posy;
    _impl->width = width;
    _impl->height = height;
    _impl->zoom = zoom;

    return EXITO;
}

int Writer::render() {
#ifdef TEXT_OPERATIVE

    if (_impl->admin != Library::I().getActualEngine())
        _impl->setAdmin(Library::I().getActualEngine());


    if ( _impl->width == 0.0 || _impl->height == 0.0)
        Screen::I().locateRenderScene(0,0,(Float)Screen::I().getWidth(),(Float)Screen::I().getHeight(),0); //
    else
        Screen::I().locateRenderScene(_impl->posx,_impl->posy,_impl->width,_impl->height,_impl->zoom);


    std::vector<int> deleteText;

    for (auto it=_impl->data->Texts.begin(),kt=_impl->data->Texts.end();it!=kt;++it) {

        auto& fx = it->second.fx;

        if (auto* l = dynamic_cast<WriterImpl::SLineText*>(it->second.Object.get())) {
            for (auto jt=l->letra.begin(),ht=l->letra.end();jt!=ht;++jt) {
                auto glyphit = l->fuente->render.find(jt->glyph);
                if (glyphit == l->fuente->render.end()) {
                    l->fuente->render.insert(std::make_pair<Uint16,Canvas>(
                        std::move(jt->glyph),
                        Canvas::createCanvas<Canvas>(TTF_RenderGlyph_Blended(l->fuente->fuente,jt->glyph,(SDL_Color)Color::White()))
                    ));
                }

                if (fx) {
                    l->fuente->render.at(jt->glyph).color(fx->red,fx->green,fx->blue,fx->alpha);
                }

                l->fuente->render.at(jt->glyph).put(jt->p);
            }

            if (fx && !fx->persistent) {
                it->second.fx.reset(nullptr);
            }
        } else if (auto* b = dynamic_cast<WriterImpl::FSTextBox*>(it->second.Object.get())) {
            if (b->update()==-1)
                deleteText.push_back(it->first);
        }
    }
    
    for (auto it=deleteText.begin(),kt=deleteText.end();it!=kt;++it) {
        auto& box = static_cast<WriterImpl::FSTextBox&>(*_impl->data->Texts.at(*it).Object);
        int aux = box.finish();
        if (aux != -1) {
            std::swap(_impl->data->Texts.at(*it),_impl->data->Texts.at(aux));
        } else {
            aux = *it;
        }

        erase(aux);

    }
    auto noadmin = _impl->session.find(nullptr);
    if (noadmin != _impl->session.end()) {
        for (auto it = noadmin->second.Texts.begin(), kt= noadmin->second.Texts.end(); it!=kt;++it) {
            if (auto* l = dynamic_cast<WriterImpl::SLineText*>(it->second.Object.get())) {
                auto& fx = it->second.fx;
                for (std::list<WriterImpl::SChar>::iterator jt=l->letra.begin(),ht=l->letra.end();jt!=ht;++jt)  {
                    if (fx)
                        l->fuente->render.at(jt->glyph).color(fx->red,fx->green,fx->blue,fx->alpha);

                    l->fuente->render.at(jt->glyph).put(jt->p);
                }

                if (fx) {
                    it->second.fx.reset(nullptr);
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

void Writer::clear() {
#ifdef TEXT_OPERATIVE
    while (!_impl->session.empty()) {
        auto it= _impl->session.begin();
        _impl->setAdmin(it->first);

        _impl->data->Texts.clear();
        _impl->data->lastIndexTextAdded.clear();

        _impl->session.erase(_impl->session.find(_impl->admin));
    }

    _impl->admin=nullptr;
    _impl->data=nullptr;

    _impl->setAdmin(nullptr);

    _impl->Fonts.clear();
    _impl->countFonts.clear();
    _impl->lastIndexFontAdded.clear();

#endif
}

#ifdef GLOBAL_SINGLETON_REFERENCES
Writer& Write = Writer::I();
#endif

} // flatscene
