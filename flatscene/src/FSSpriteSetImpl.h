#ifndef __FS_SPRITESET_IMPL__
#define __FS_SPRITESET_IMPL__

#include "FSSpriteSet.h"
#include "FSException.h"
#include "FSparserXML.h"
#include "FSLibrary.h"
#include "debugfuncs.h"

#include <algorithm>
#include <iostream>

#include "XMLHelper.h"

using namespace fs::intern::xml;

struct FSSpriteset::SpritesetImpl {

    unsigned int globalAuxiliar;

    SpritesetImpl(std::string c, Uint8 mode) 
        : globalAuxiliar(0), mode(mode), name(c) {
            loadChipset(c,mode);
    }

    SpriteCollection m_vecSprites ;
    std::string name;
    Uint8 mode;

    Uint8 getMode() {
        return mode;
    }

    const std::string& getName() const {
        return name;
    }

    bool setName(std::string& name) {
        if (this->name=="") {
            this->name=name;
            return true;
        }
        return false;
    }

    void add ( FSSprite* pspt ) {
        m_vecSprites.push_back ( pspt ) ;
    }

    bool has ( FSSprite* pspt ) const {
        const auto iter = std::find ( m_vecSprites.begin ( ) , m_vecSprites.end ( ) , pspt ) ;
        return ( iter != m_vecSprites.end ( ) ) ;
    }

    int search( FSSprite* pspt ) const {
        const auto iter = std::find ( m_vecSprites.begin ( ) , m_vecSprites.end ( ) , pspt ) ;
        if ( iter == m_vecSprites.end ( ) ) return ( -1 ) ;
        return ( iter - m_vecSprites.begin ( ) ) ;
    }

    void remove ( FSSprite* pspt ) {
        SpriteCollection::iterator iter = std::find ( m_vecSprites.begin ( ) , m_vecSprites.end ( ) , pspt ) ;
        if ( iter == m_vecSprites.end ( ) ) return ;
        m_vecSprites.erase ( iter ) ;
    }

    FSSprite* get ( unsigned int n ) const {
        if ( n < m_vecSprites.size()) {
            return ( m_vecSprites.at(n) ) ;
        } else {
            return NULL;
        }
    }
    int size ( ) const {
        return ( m_vecSprites.size ( ) ) ;
    }

    void loadChipset(std::string& c,Uint8 mode=ONLY_TEXTURE,std::string* cPrev=NULL) {


        int num_img=0;
        int ancho=0;
        int alto=0;

        int sp_scale = 1;

        SDL_Surface* chipset = NULL;

        bool simple = false;
        globalAuxiliar++;   // En cada recursi�n del m�todo lo incrementamos.

        std::string s_aux=c;
        std::string tipefile("");
        std::string namefile("");


        // Descomponemos el descriptor del archivo

        for (int i=s_aux.size()-1;i>=0;i--) {
            char letra = s_aux.at(i);

            tipefile = letra + tipefile;
            s_aux = s_aux.substr(0,s_aux.size()-1);

            if (letra=='.')
                break;
        }

        if (s_aux.empty())
            s_aux=c;

        if (cPrev) { // Es decir, si ha habido recursi�n debido a la funci�n defined-in, caso especial.
            namefile = *cPrev;
            tipefile="";

            for (int i=namefile.size()-1;i>=0;i--) {
                char letra = namefile.at(i);

                tipefile = letra + tipefile;
                namefile = namefile.substr(0,namefile.size()-1);

                if (letra=='.')
                    break;
            }

            if (namefile.empty()) {
                namefile = *cPrev;
                tipefile = ".grd";
            }

        } else {
            namefile = s_aux;

            if (tipefile == s_aux)
                tipefile = ".png";

        }
    
        s_aux+=".grd";

        SDL_Surface chipTest;
        chipTest.w = 640;
        loadFileGRD(s_aux,&chipTest);
        exit(0);

        #ifdef LOG_SPRITESET
            if (cPrev == NULL)
                printf("\nSpriteset %s\n",c.c_str());
        #endif

        // Abrimos el .grd y reescatamos la estructura de datos.

        TiXmlDocument xmldoc(s_aux.c_str());
        if (!xmldoc.LoadFile()) {       // Cargamos el .grd
            // Si no carga, asumimos que no existe el .grd y el Spriteset ser� un �nico Sprite sin informaci�n adicional

            if (cPrev || tipefile == ".grd") // en ese caso no tiene sentido que haya varios ciclos
                 return FSLibrary::I().Error(s_aux,TE_fileExists);

            num_img = 1;

            s_aux=namefile+tipefile;

            chipset=IMG_Load(s_aux.c_str());
            if (!chipset) { FSLibrary::I().Error(s_aux.c_str(),TE_fileExists); }

            ancho = chipset->w;
            alto = chipset->h;

            if (SDL_GetVideoSurface() != NULL) {

                SDL_Surface* sdl_surf=SDL_DisplayFormat(chipset);
                if (sdl_surf) {
                    SDL_FreeSurface(chipset);
                    chipset = sdl_surf;
                } else {
                    return FSLibrary::I().Error("SDL_DisplayFormat(chipset) fallo.");
                }

            } else if (mode != ONLY_SDL_SURFACE)
                return FSLibrary::I().Error("Librer�a no inicializada antes de crear CSpriteSet con texturas. Usa ONLY_SDL_SURFACE.");

            FSSprite* pspt=new FSSprite(FSCanvas::toSCanvas(chipset,mode));

            pspt->setName(s_aux.c_str());

            add(pspt);

            return;

        } 

        if (cPrev)
            c = *cPrev; // Si este archivo ha sido enlazado desde otro .grd, sustituimos el nombre por si la recursi�n continua.

        if (globalAuxiliar > 100)
            return FSLibrary::I().Error(("Estructura defectuosa del archivo: "+c+". \nDependencias c�clicas.").c_str());  // Si la recursi�n es demasiado grande, ERROR.

        TiXmlHandle input(xmldoc.FirstChild());

        if (!input.ToElement()) { 
            return FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux).c_str());
        }

        if (input.ToElement()->Attribute("defined-in")) {
            std::string sdefined(input.ToElement()->Attribute("defined-in"));
            return loadChipset(sdefined,mode,&c);
        }

        if (!input.ToElement()->Attribute("sprites")) {
            return FSLibrary::I().Error("Estructura defectuosa para el archivo: "+s_aux+"\nNo se ha especificado el n�mero de sprites.");
        }

        if (input.ToElement()->Attribute("type") && strcmp(input.ToElement()->Attribute("type"),"split")==0)
            return loadChipsetSplit(s_aux,mode);

        if (!(input.ToElement()->Attribute("cellwidth") && input.ToElement()->Attribute("cellheight"))) { 
            return FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nNo se ha especificado dimensiones para las celdas.").c_str());
        }

        input.Element()->QueryIntAttribute("sprites",&num_img); // Tenemos el n�mero de im�genes.
        input.Element()->QueryIntAttribute("cellwidth",&ancho); // Tenemos el ancho de celda.
        input.Element()->QueryIntAttribute("cellheight",&alto);// Tenemos el alto de celda.


        if (input.ToElement()->Attribute("simple") && strcmp(input.ToElement()->Attribute("simple"),"true")==0)
            simple=true;    // Si el spriteset es de tipo simple, activamos este booleano.

        if (input.Element()->Attribute("sp-scale"))
            input.Element()->QueryIntAttribute("sp-scale",&sp_scale); // Hay escalado predefinido de SDL_Surface

        // Primer lote de la estructura de datos procesado
        // Ahora se empieza a carga la imagen con los datos obtenidos

        if (tipefile == ".png" || tipefile == ".bmp" || tipefile == ".jpg2") {
            s_aux=namefile + tipefile;

            chipset=IMG_Load(s_aux.c_str());
            if (!chipset) {
                return FSLibrary::I().Error(s_aux,TE_fileExists);
            }
        } else if (tipefile == ".grd") {

            s_aux=namefile + ".png";

            chipset=IMG_Load(s_aux.c_str());
            if (!chipset) {
                s_aux=namefile + ".jpg";
                chipset=IMG_Load(s_aux.c_str());
                if (!chipset) {
                    s_aux=namefile + ".bmp";
                    chipset=IMG_Load(s_aux.c_str());
                    if (!chipset) {
                        return FSLibrary::I().Error(s_aux,TE_fileExists);
                    }
                }
            }
        } else {
            return FSLibrary::I().Error(s_aux + "No es un tipo de recurso grafico apropiado.",TE_standard);
        }

        SDL_Surface* sdl_surf = NULL;

        /*SDL_PixelFormat* f = chipset->format;
        printf("bpp:%d Amask:%X alpha:%d colorkey:%d Rmask:%X Gmask:%X Bmask:%X\n",f->BitsPerPixel,f->Amask,f->alpha,f->colorkey,f->Rmask,f->Gmask,f->Bmask);
        */

        Uint32 columnas = chipset->w / ancho;       // Calculamos el n�mero de columnas del spriteset.

        if (    (((float)columnas) - (((float)chipset->w)/((float)ancho))) != 0.00 || (     ((float)(chipset->h / alto))    -  (((float)chipset->h)/((float)alto))      )!=0.00 )   {
            return FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nEl tama�o de la imagen debe ser m�ltiplo exacto de las celdas.").c_str());
        }

        if (columnas <= 0 || (int)columnas > chipset->w) { 
            return FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nCantidad de columnas imposible.").c_str());
        }

    
        // Empezamos a preparar el formato de la imagen.

    

        if (SDL_GetVideoSurface() != NULL) {

            sdl_surf=SDL_DisplayFormatAlpha(chipset);
            if (sdl_surf) {
                SDL_FreeSurface(chipset);
                chipset=sdl_surf;
            } else {
                return FSLibrary::I().Error("SDL_DisplayFormat(chipset) fallo.");
            }

        } else if (mode != ONLY_SDL_SURFACE) 
            return FSLibrary::I().Error("Librer�a no inicializada antes de crear CSpriteSet con texturas. Usa ONLY_SDL_SURFACE.");
    
        s_aux.clear();

        SDL_Rect rect; // Este rectangulo llevara la cuenta de las coordenadas por la que nos encontramos dentro del spriteset (chipset).
        rect.x=0;
        rect.y=0;

        FSSprite* m_pSprite;

        struct { int w,h,nump; const char *name; } iinfo;

        TiXmlElement* img=NULL;

        FSPoint globalCP;   // El punto central global es opcional, y si no se encuentra, se le asigna -1 a X.
        globalCP.setX(-1);
        std::map<int,RectsInfo> globalAreas;

        s_aux = namefile + ".grd";

        // Otro lote de la estructura de datos necesario para recortar la imagen.

        if (!simple) {      // Si el spriteset no es simple...
            img = input.FirstChildElement("globalcpoint").ToElement();
            if (img) {  // Si hay punto central global...
                if (!(img->Attribute("x") && img->Attribute("y") &&
                    atoi(img->Attribute("x"))>=0 && atoi(img->Attribute("x"))<=ancho &&
                    atoi(img->Attribute("y"))>=0 && atoi(img->Attribute("y"))<=alto))
                        FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores de punto centro global err�neos.").c_str());
                globalCP.set(atoi(img->Attribute("x"))*sp_scale,atoi(img->Attribute("y"))*sp_scale);    // Lo asignamos.
            }

            img = input.FirstChildElement("globalareas").FirstChildElement("area").ToElement();
            while (img) {   // Mientras haya areas globales...
                if (!img->Attribute("id") || atoi(img->Attribute("id"))<0)
                    FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores de areas globales err�neos.").c_str());
                int idArea = atoi(img->Attribute("id")); // Determinamos que n�mero de area es (las areas van ordenadas segun el conjunto de los numeros naturales, de forma continua y orden ascendente).

                if (img->Attribute("relative") && strcmp(img->Attribute("relative"),"true")==0) // Miramos si las coordenadas de esa area estan descritas en terminos relativos (segun el punto de centro) o absolutos.
                    globalAreas[idArea].relative=true;  // Y la a�adimos a la posicion del mapa correspondiente
                else
                    globalAreas[idArea].relative=false;

                for (TiXmlElement* rectNode=img->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement("rectangle")) {      // Por cada rectangulo de esa area...
                    if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2")))
                        FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rect�ngulo un �rea global err�neos.").c_str());

                    SDL_Rect_Signed rc;
                    rc.x = atoi(rectNode->Attribute("x1")) *sp_scale;
                    rc.w = atoi(rectNode->Attribute("x2")) *sp_scale;
                    rc.y = atoi(rectNode->Attribute("y1")) *sp_scale;
                    rc.h = atoi(rectNode->Attribute("y2")) *sp_scale;

                    globalAreas[idArea].v.push_back(rc);    // Y la a�adimos al vector pertinente del mapa, para luego reescatarla directamente del mapa.
                }

                img = img->NextSiblingElement("area");
            }

            img = input.FirstChildElement("img").ToElement();       // Preparamos el puntero al siguiente nodo imagen.
        } else {
            #ifdef LOG_SPRITESET_INFO
                printf("%d Simple Sprites\n",num_img);
            #endif
        }

        // Recorte de cada fragmento de la imagen

        for (int i=0;img || ( simple && (i<num_img) );i++) {    // Mientras haya un nodo imagen siguiente o si el spriteset es simple y segun la cuenta aun no hemos llegado a la �ltima imagen...
            if (!simple && img->Attribute("name")) {    // Asignamos nombre si tiene, si no "noname".
                iinfo.name = img->Attribute("name");
            } else {
                iinfo.name = "noname";
            }
            #ifdef LOG_SPRITESET_INFO
                if (!simple) printf("Image%d %s\n",i,iinfo.name);
            #endif
            if (!simple && img->Attribute("width")) {   // Asignamos al rectangulo de coordenas la anchura. Si no tiene especifica, no por defecto es el ancho de celda.
                iinfo.w = atoi(img->Attribute("width"));
                if (iinfo.w > ancho || iinfo.w <= 0)        {   FSLibrary::I().Error("Estructura defectuosa del archivo: "+s_aux+"\nValores en la imagen "+iinfo.name+" err�neos.");  }
            }   else
                iinfo.w = ancho;
            if (!simple && img->Attribute("height")) { // Asignamos al rectangulo de coordenas la altura. Si no tiene especifica, no por defecto es el alto de celda.
                iinfo.h =   atoi(img->Attribute("height"));
                if (iinfo.h > alto || iinfo.h <= 0) {   FSLibrary::I().Error("Estructura defectuosa del archivo: "+s_aux+"\nValores en la imagen "+iinfo.name+" err�neos.");  }
            }   else
                iinfo.h = alto;

            if (((i%columnas)==0) && (i!=0)) { // Si pasamos de la �ltima columna, volvemos a la columna 0 y avanzamos 1 fila.
                rect.x=0;
                rect.y+=alto;

            }
            // Desplazamos las coordenadas hacia la siguiente celda.
            rect.h=rect.y+iinfo.h;
            rect.w=rect.x+iinfo.w;

            SDL_Surface* temp;

            sdl_surf = SDL_CreateRGBSurface(chipset->flags | SDL_SRCALPHA,iinfo.w,iinfo.h,chipset->format->BitsPerPixel,chipset->format->Rmask,chipset->format->Gmask,chipset->format->Bmask,chipset->format->Amask);   // Creamos una surface que contendra la informacion de la celda actual.
            //sdl_surf = SDL_CreateRGBSurface(chipset->flags | SDL_SRCALPHA,iinfo.w,iinfo.h,chipset->format->BitsPerPixel,0xFF,0xFF00,0xFF0000,0xFF000000); // Creamos una surface que contendra la informacion de la celda actual.
            //sdl_surf = SDL_CreateRGBSurface(chipset->flags | SDL_SRCALPHA,iinfo.w,iinfo.h,chipset->format->BitsPerPixel,0,0,0,0); // Creamos una surface que contendra la informacion de la celda actual.

            SDL_SetColorKey(sdl_surf,SDL_SRCCOLORKEY, chipset->format->colorkey);   // Le asignamos los formatos de la surface del spriteset.

            //SDL_BlitSurface(chipset,&rect,sdl_surf,0);

            blitcopy(chipset,&rect,sdl_surf,NULL);

            //countalpha(sdl_surf);


            if (sp_scale > 0 && sp_scale != 1.0 && mode != ONLY_SDL_SURFACE) {
                temp = FSCanvas::scaleSurface(sdl_surf,sp_scale);
                SDL_FreeSurface(sdl_surf);
                sdl_surf=temp;
                SDL_SetColorKey(sdl_surf,SDL_SRCCOLORKEY,chipset->format->colorkey);        // Reasignamos los formatos.
            }

            SCanvas m_pImage = FSCanvas::toSCanvas(sdl_surf,mode);      // Convertimos la SDL_Surface en SCanvas(

            //Imagen creada, ahora el resto de su estructura de datos.

            TiXmlElement* cpoint=NULL;
            if (!simple && (globalCP.x<0)) // Y ahora si el spriteset no es simple y no existe globalcpoint, intentamos rescatar el cpoint.
                cpoint = input.ChildElement("img",i).FirstChildElement("cpoint").ToElement();

            if (cpoint) {   // Si es procedente rescatar el cpoint...
                #ifdef LOG_SPRITESET_INFO
                    printf("\tCP : %d,%d\n",atoi(cpoint->Attribute("x"))*sp_scale,atoi(cpoint->Attribute("y"))*sp_scale);
                #endif
                if (!(cpoint->Attribute("x") && cpoint->Attribute("y") &&
                    atoi(cpoint->Attribute("x"))>=0 && atoi(cpoint->Attribute("x"))<=iinfo.w &&
                    atoi(cpoint->Attribute("y"))>=0 && atoi(cpoint->Attribute("y"))<=iinfo.h))
                        FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en el punto centro de la imagen "+iinfo.name+" err�neos.").c_str());
                m_pSprite=new FSSprite(m_pImage,FSPoint(atoi(cpoint->Attribute("x"))*sp_scale,atoi(cpoint->Attribute("y"))*sp_scale));
            } else if (globalCP.x>=0) { // Si existe un global cpoint...
                m_pSprite=new FSSprite(m_pImage,FSPoint(globalCP));
            } else {
                m_pSprite=new FSSprite(m_pImage);   // Acabamos llamando al constructor del sprite, con o sin cpoint.
            }

            TiXmlElement* area = NULL;
            if (!simple)
                area = input.ChildElement("img",i).FirstChildElement("area").ToElement(); // Si el spriteset no es simple, buscamos areas en la imagen.

            for (unsigned int numArea=0,countGlobalAreas=0;(countGlobalAreas < globalAreas.size()) || (area && area->Attribute("id"));numArea++) {  // Si hay...
                RectArea rArea; // Preparamos la nueva area.
            #ifdef LOG_SPRITESET_INFO
                int idebug=0;
            #endif
                if (globalAreas.find(numArea)==globalAreas.end()) { // Si esta area no se corresponde con ninguna de las areas globales...
                    if (!area)  // Si no existe area, es que las areas globales estan mal definidas en el fichero .grd
                        FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nSe requiere un orden ascendente coherente entre areas (sin saltos) teniendo en cuenta las areas globales.").c_str());
                    bool relative = area->Attribute("relative") && strcmp(area->Attribute("relative"),"true")==0;   // Rescatamos si las coordenadas son relativas.
                    for (TiXmlElement* rectNode=area->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement("rectangle")) { // Por cada rectangulo perteneciente al area.
                        if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2")))
                            FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rect�ngulo de la imagen "+iinfo.name+" err�neos.").c_str());

                        SDL_Rect_Signed rc;
                        rc.x = atoi(rectNode->Attribute("x1")); // Anotamos las coordenadas.
                        rc.w = atoi(rectNode->Attribute("x2"));
                        rc.y = atoi(rectNode->Attribute("y1"));
                        rc.h = atoi(rectNode->Attribute("y2"));
                        if (!relative) {    // Si son relativas, tenemos en cuentra el punto de centro.
                            rc.x -= m_pSprite->getCenter().getX();
                            rc.w -= m_pSprite->getCenter().getX();
                            rc.y -= m_pSprite->getCenter().getY();
                            rc.h -= m_pSprite->getCenter().getY();
                        }

                        if (/*(rc.x < 0) || (rc.x > iinfo.w) || (rc.y < 0) || (rc.y > iinfo.w) || (rc.w < 0) || (rc.w > iinfo.h) || (rc.h < 0) || (rc.h > iinfo.h) || */(rc.w<rc.x) || (rc.h<rc.y))
                            FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rect�ngulo de la imagen "+iinfo.name+" conflictivos.").c_str());

                        rArea.push_back(FSRectangle(rc.x *sp_scale,rc.y *sp_scale,rc.w *sp_scale,rc.h *sp_scale)); // Finalmente creamos con ellas un rectangulo y lo a�adimos al area actual.
                    #ifdef LOG_SPRITESET_INFO
                        printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x*sp_scale,rc.w*sp_scale,rc.y*sp_scale,rc.h*sp_scale);
                        idebug++;
                    #endif
                    }
                    area=area->NextSiblingElement("area");
                } else {        //Si es un area global.
                    for (std::vector<SDL_Rect_Signed>::iterator it=globalAreas[numArea].v.begin();it!=globalAreas[numArea].v.end();++it) {
                        SDL_Rect_Signed rc = *it;   // Reescatamos la informacion de las coordenadas anotada anteriormente y guardado en el vector perteneciente a el pertinente puesto del mapa.
                        if (!globalAreas[numArea].relative) {   // Si son relativas, tenemos en cuenta el punto de centro.
                            rc.x -= m_pSprite->getCenter().getX();
                            rc.w -= m_pSprite->getCenter().getX();
                            rc.y -= m_pSprite->getCenter().getY();
                            rc.h -= m_pSprite->getCenter().getY();
                        }
                        if (/*(rc.x < 0) || (rc.x > iinfo.w) || (rc.y < 0) || (rc.y > iinfo.w) || (rc.w < 0) || (rc.w > iinfo.h) || (rc.h < 0) || (rc.h > iinfo.h) ||*/ (rc.w<rc.x) || (rc.h<rc.y))
                            FSLibrary::I().Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rect�ngulo global aplicado la imagen "+iinfo.name+" conflictivos.").c_str());

                        rArea.push_back(FSRectangle(rc.x*sp_scale,rc.y*sp_scale,rc.w*sp_scale,rc.h*sp_scale)); // Finalmente creamos con ellas un rectangulo y lo a�adimos al area actual.
                    #ifdef LOG_SPRITESET_INFO
                        printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x*sp_scale,rc.w*sp_scale,rc.y*sp_scale,rc.h*sp_scale);
                        idebug++;
                    #endif
                    }
                    countGlobalAreas++;
                }
                m_pSprite->addArea(rArea);  // Finalmente a�adimos el area a este sprite.
            }


            m_pSprite->setName(iinfo.name); // Asignamos el nombre al sprite reciencreado.

            add(m_pSprite); //A�adimos el sprite a este objeto cspriteset.

            rect.x+=ancho;  // Aumentamos la coordenada hacia la siguiente columna.

            if (!simple)
                img=img->NextSiblingElement("img"); // Si no es simple, buscamos la siguiente etiqueta imagen.
        }
        SDL_FreeSurface(chipset);
        chipset=NULL;

    }

    void loadChipsetSplit(std::string grd,Uint8 mode=ONLY_TEXTURE) {

        int num_img=0;
        int ancho=0;
        int alto=0;

        int sp_scale = 1;

        std::string folder = grd;

        for (int i=folder.size()-1;i>=0;i--) {
            char letra = folder.at(i);

            folder = folder.substr(0,folder.size()-1);

            if (letra=='/' || letra=='\\')
                break;
        }

        //if (folder.empty())
        //  folder = ".";

        TiXmlDocument grddoc(grd.c_str());

        if (!grddoc.LoadFile())
            FSLibrary::I().Error(grd.c_str(),TE_fileExists);

        TiXmlElement* head = grddoc.FirstChildElement("Spriteset");

        if (!head || !head->Attribute("type") || strcmp(head->Attribute("type"),"split")!=0)
            return FSLibrary::I().Error("Estructura defectuosa para el archivo: "+grd+". Deber�a tratarse de un 'grd' de tipo 'split' formal.");

        head->QueryIntAttribute("sprites",&num_img);

        if (head->Attribute("sp-scale"))
            head->QueryIntAttribute("sp-scale",&sp_scale); // Hay escalado predefinido de SDL_Surface


        FSPoint globalCP;   // El punto central global es opcional, y si no se encuentra, se le asigna -1 a X.
        globalCP.setX(-1);
        std::map<int,RectsInfo> globalAreas;

        TiXmlHandle input = grddoc.FirstChild();

        TiXmlElement* elmnt = input.FirstChildElement("globalcpoint").ToElement();

        if (elmnt) {    // Si hay punto central global...
            if (!(elmnt->Attribute("x") && elmnt->Attribute("y") &&
                atoi(elmnt->Attribute("x"))>=0 && atoi(elmnt->Attribute("x"))<=ancho &&
                atoi(elmnt->Attribute("y"))>=0 && atoi(elmnt->Attribute("y"))<=alto))
                    FSLibrary::I().Error(("Estructura defectuosa del archivo: "+grd+"\nValores de punto centro global err�neos.").c_str());
            globalCP.set(atoi(elmnt->Attribute("x"))*sp_scale,atoi(elmnt->Attribute("y"))*sp_scale);    // Lo asignamos.
        }

        elmnt = input.FirstChildElement("globalareas").FirstChildElement("area").ToElement();
        while (elmnt) { // Mientras haya areas globales...
            if (!elmnt->Attribute("id") || atoi(elmnt->Attribute("id"))<0)
                FSLibrary::I().Error(("Estructura defectuosa del archivo: "+grd+"\nValores de areas globales err�neos.").c_str());
            int idArea = atoi(elmnt->Attribute("id")); // Determinamos que n�mero de area es (las areas van ordenadas segun el conjunto de los numeros naturales, de forma continua y orden ascendente).

            if (elmnt->Attribute("relative") && strcmp(elmnt->Attribute("relative"),"true")==0) // Miramos si las coordenadas de esa area estan descritas en terminos relativos (segun el punto de centro) o absolutos.
                globalAreas[idArea].relative=true;  // Y la a�adimos a la posicion del mapa correspondiente
            else
                globalAreas[idArea].relative=false;

            for (TiXmlElement* rectNode=elmnt->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement("rectangle")) {        // Por cada rectangulo de esa area...
                if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2")))
                    FSLibrary::I().Error(("Estructura defectuosa del archivo: "+grd+"\nValores en un rect�ngulo un �rea global err�neos.").c_str());

                SDL_Rect_Signed rc;
                rc.x = atoi(rectNode->Attribute("x1")) *sp_scale;
                rc.w = atoi(rectNode->Attribute("x2")) *sp_scale;
                rc.y = atoi(rectNode->Attribute("y1")) *sp_scale;
                rc.h = atoi(rectNode->Attribute("y2")) *sp_scale;

                globalAreas[idArea].v.push_back(rc);    // Y la a�adimos al vector pertinente del mapa, para luego reescatarla directamente del mapa.
            }

            elmnt = elmnt->NextSiblingElement("area");

        }

        SDL_Rect rect; // Este rectangulo llevara la cuenta de las coordenadas por la que nos encontramos dentro del spriteset (chipset).
        rect.x=0;
        rect.y=0;

        FSSprite* m_pSprite;

        struct { int w,h,nump; const char *name; } iinfo;

        elmnt=input.FirstChildElement("img").ToElement();

        while (elmnt != NULL) { // Mientras haya un nodo imagen siguiente

            if (elmnt->Attribute("name"))
                iinfo.name = elmnt->Attribute("name");
            else
                iinfo.name = "noname";

            if (elmnt->Attribute("ref")) {

                SCanvas m_pImage;

                SDL_Surface* temp = NULL, *sdl_surf = IMG_Load((folder+elmnt->Attribute("ref")).c_str());

                if (sdl_surf) {

                    if (SDL_GetVideoSurface() != NULL) {

                        temp=SDL_DisplayFormatAlpha(sdl_surf);
                        if (temp) {
                            SDL_FreeSurface(sdl_surf);
                            sdl_surf = temp;
                        } else {
                            return FSLibrary::I().Error("SDL_DisplayFormat(chipset) fallo.");
                        }

                    } else if (mode != ONLY_SDL_SURFACE)
                        return FSLibrary::I().Error("Librer�a no inicializada antes de crear CSpriteSet con texturas. Usa ONLY_SDL_SURFACE.");

                    if (sp_scale > 0 && sp_scale != 1.0 && mode != ONLY_SDL_SURFACE) {
                        temp = FSCanvas::scaleSurface(sdl_surf,sp_scale);
                        SDL_FreeSurface(sdl_surf);
                        sdl_surf=temp;
                        SDL_SetColorKey(sdl_surf,SDL_SRCCOLORKEY,sdl_surf->format->colorkey);       // Reasignamos los formatos.
                    }



                    m_pImage = FSCanvas::toSCanvas(sdl_surf,mode);

                    TiXmlElement* cpoint = NULL;

                    if (globalCP.x < 0)
                        cpoint = elmnt->FirstChildElement("cpoint");

                    if (cpoint) {
                        if (!(cpoint->Attribute("x") && cpoint->Attribute("y") &&
                            atoi(cpoint->Attribute("x"))>=0 && atoi(cpoint->Attribute("x"))<=m_pImage.w2 &&
                            atoi(cpoint->Attribute("y"))>=0 && atoi(cpoint->Attribute("y"))<=m_pImage.h2))
                                FSLibrary::I().Error(("Estructura defectuosa del archivo: "+grd+"\nValores en el punto centro de la imagen "+iinfo.name+" err�neos.").c_str());
                        m_pSprite=new FSSprite(m_pImage,FSPoint(atoi(cpoint->Attribute("x"))*sp_scale,atoi(cpoint->Attribute("y"))*sp_scale));
                    } else if (globalCP.x>=0) { // Si existe un global cpoint...
                        m_pSprite=new FSSprite(m_pImage,FSPoint(globalCP));
                    } else {
                        m_pSprite=new FSSprite(m_pImage);   // Acabamos llamando al constructor del sprite, con o sin cpoint.
                    }

                }

            }


            TiXmlElement* area = elmnt->FirstChildElement("area"); // Si el spriteset no es simple, buscamos areas en la imagen.

            for (unsigned int numArea=0,countGlobalAreas=0;(countGlobalAreas < globalAreas.size()) || (area && area->Attribute("id"));numArea++) {  // Si hay...
                RectArea rArea; // Preparamos la nueva area.
            #ifdef LOG_SPRITESET_INFO
                int idebug=0;
            #endif
                if (globalAreas.find(numArea)==globalAreas.end()) { // Si esta area no se corresponde con ninguna de las areas globales...
                    if (!area)  // Si no existe area, es que las areas globales estan mal definidas en el fichero .grd
                        FSLibrary::I().Error(("Estructura defectuosa del archivo: "+grd+"\nSe requiere un orden ascendente coherente entre areas (sin saltos) teniendo en cuenta las areas globales.").c_str());
                    bool relative = area->Attribute("relative") && strcmp(area->Attribute("relative"),"true")==0;   // Rescatamos si las coordenadas son relativas.
                    for (TiXmlElement* rectNode=area->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement("rectangle")) { // Por cada rectangulo perteneciente al area.
                        if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2")))
                            FSLibrary::I().Error(("Estructura defectuosa del archivo: "+grd+"\nValores en un rect�ngulo de la imagen "+iinfo.name+" err�neos.").c_str());

                        SDL_Rect_Signed rc;
                        rc.x = atoi(rectNode->Attribute("x1")); // Anotamos las coordenadas.
                        rc.w = atoi(rectNode->Attribute("x2"));
                        rc.y = atoi(rectNode->Attribute("y1"));
                        rc.h = atoi(rectNode->Attribute("y2"));
                        if (!relative) {    // Si son relativas, tenemos en cuentra el punto de centro.
                            rc.x -= m_pSprite->getCenter().getX();
                            rc.w -= m_pSprite->getCenter().getX();
                            rc.y -= m_pSprite->getCenter().getY();
                            rc.h -= m_pSprite->getCenter().getY();
                        }

                        if (/*(rc.x < 0) || (rc.x > iinfo.w) || (rc.y < 0) || (rc.y > iinfo.w) || (rc.w < 0) || (rc.w > iinfo.h) || (rc.h < 0) || (rc.h > iinfo.h) || */(rc.w<rc.x) || (rc.h<rc.y))
                            FSLibrary::I().Error(("Estructura defectuosa del archivo: "+grd+"\nValores en un rect�ngulo de la imagen "+iinfo.name+" conflictivos.").c_str());

                        rArea.push_back(FSRectangle(rc.x *sp_scale,rc.y *sp_scale,rc.w *sp_scale,rc.h *sp_scale)); // Finalmente creamos con ellas un rectangulo y lo a�adimos al area actual.
                    #ifdef LOG_SPRITESET_INFO
                        printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x*sp_scale,rc.w*sp_scale,rc.y*sp_scale,rc.h*sp_scale);
                        idebug++;
                    #endif
                    }
                    area=area->NextSiblingElement("area");
                } else {        //Si es un area global.
                    for (std::vector<SDL_Rect_Signed>::iterator it=globalAreas[numArea].v.begin();it!=globalAreas[numArea].v.end();++it) {
                        SDL_Rect_Signed rc = *it;   // Reescatamos la informacion de las coordenadas anotada anteriormente y guardado en el vector perteneciente a el pertinente puesto del mapa.
                        if (!globalAreas[numArea].relative) {   // Si son relativas, tenemos en cuenta el punto de centro.
                            rc.x -= m_pSprite->getCenter().getX();
                            rc.w -= m_pSprite->getCenter().getX();
                            rc.y -= m_pSprite->getCenter().getY();
                            rc.h -= m_pSprite->getCenter().getY();
                        }
                        if (/*(rc.x < 0) || (rc.x > iinfo.w) || (rc.y < 0) || (rc.y > iinfo.w) || (rc.w < 0) || (rc.w > iinfo.h) || (rc.h < 0) || (rc.h > iinfo.h) ||*/ (rc.w<rc.x) || (rc.h<rc.y))
                            FSLibrary::I().Error(("Estructura defectuosa del archivo: "+grd+"\nValores en un rect�ngulo global aplicado la imagen "+iinfo.name+" conflictivos.").c_str());

                        rArea.push_back(FSRectangle(rc.x*sp_scale,rc.y*sp_scale,rc.w*sp_scale,rc.h*sp_scale)); // Finalmente creamos con ellas un rectangulo y lo a�adimos al area actual.
                    #ifdef LOG_SPRITESET_INFO
                        printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x*sp_scale,rc.w*sp_scale,rc.y*sp_scale,rc.h*sp_scale);
                        idebug++;
                    #endif
                    }
                    countGlobalAreas++;
                }
                m_pSprite->addArea(rArea);  // Finalmente a�adimos el area a este sprite.
            }


            m_pSprite->setName(iinfo.name); // Asignamos el nombre al sprite reciencreado.

            add(m_pSprite); //A�adimos el sprite a este objeto cspriteset.

            elmnt = elmnt->NextSiblingElement("img");   // Si no es simple, buscamos la siguiente etiqueta imagen.
        }

    }

    bool isValidBitmapExtension(const std::string& bitmap) {
        return bitmap == ".png" || bitmap == ".bmp" || bitmap == ".jpg";
    }

    void getNameFile(const std::string& str, std::string& grd, std::string& bitmap) {

        std::string tipefile;
        std::string namefile;

        auto res = str.end();
        for(auto it=str.begin(), et=str.end(); it != et ; ++it)
            if (*it == '.')
                res = it;

        if (res != str.end()) {
            namefile = std::string(str.begin(),res);
            tipefile = std::string(res,str.end());
        } else {
            namefile = str;
            tipefile = ".png";
        }

        if (!isValidBitmapExtension(tipefile)) 
            throw FSException("graphic bitmap format not valid");

        grd = namefile + ".grd";
        bitmap = namefile + tipefile;

    }

    struct DataGRD {
        struct Area {
            bool relative;
            std::vector<FSRectangle> rc;
        };
        struct Sprite {
            std::string name;
            FSPoint dim;
            FSPoint cp;
            std::map<int,Area> areas;
        };
        std::vector<Sprite> images;
        unsigned int num_img;
        unsigned int cellwidth;
        unsigned int cellheight;
        unsigned int columns;
        bool simple;
        double sp_scale;
        FSPoint globalcp;
        std::map<int,Area> globalareas;
    };

    DataGRD loadFileGRD(const std::string& grd_str, const SDL_Surface *const chipset = nullptr) {
        TiXmlDocument doc(grd_str.c_str());
        if (!doc.LoadFile()) {
            if (!chipset) 
                throw FSException("grd file invalid and bitmap invalid",__LINE__);

            return fillGRDFromChipset(*chipset);
        }
        return fillGRDFromDocument(doc);
    }

    DataGRD fillGRDFromDocument(TiXmlDocument& doc) {
        DataGRD grd;

        TiXmlHandle input(doc.FirstChild()); 
        if (!input.Element()) throw FSException("no elements in grd file",__LINE__);

        auto& head = *input.Element();
        if (isDefinedInOtherFile(head))
            return getFromOtherFile(head);

        processHeadElement(grd,head);
        processGlobalValues(grd,input);
        processSpriteValues(grd,input);

        return grd;
    }

    DataGRD fillGRDFromChipset(const SDL_Surface& chipset) {
        DataGRD grd;
        grd.images.push_back(DataGRD::Sprite());

        auto& img = grd.images.front();
        img.dim.x = chipset.w;
        img.dim.y = chipset.h;

        return grd;
    }

    void fillAreasFromElement(const TiXmlElement* pArea, std::map<int,DataGRD::Area>& areas) {
        for ( ; pArea ; pArea = pArea->NextSiblingElement()) {
            int id = numFromAttr(*pArea,"id",0);

            auto& area = areas.at(id);
            area.relative = checkAttr(*pArea,"relative","true");
            for (auto pRect = pArea->FirstChildElement("rectangle"); pRect; pRect = pRect->NextSiblingElement())
                area.rc.push_back(((int) (intFromAttr(*pRect,"x1") ) ,
                                   (int) (intFromAttr(*pRect,"x2") ) ,
                                   (int) (intFromAttr(*pRect,"y1") ) ,
                                   (int) (intFromAttr(*pRect,"y2"))));
        }
    }

    bool isDefinedInOtherFile(const TiXmlElement& head) {
        return checkAttr(head,"defined-in","",false);
    }

    DataGRD getFromOtherFile(const TiXmlElement& head) {
        ensureAttr(head,"defined-in","",false);
        std::string grd_str(head.Attribute("defined-in"));
        return loadFileGRD(grd_str);
    }

    void processHeadElement(DataGRD& grd, const TiXmlElement& head) {
        grd.num_img    = numFromAttr<decltype(grd.num_img)   >(head,"sprites");
        grd.cellwidth  = numFromAttr<decltype(grd.cellwidth) >(head,"cellwidth");
        grd.cellheight = numFromAttr<decltype(grd.cellheight)>(head,"cellheight");

        if (checkAttr(head,"type","split"))
            ; // @TODO return loadChipsetSplit(s_aux,mode);

        grd.simple = checkAttr(head,"simple","true");
        if (checkAttr(head,"sp-scale"))
            grd.sp_scale = numFromAttr<decltype(grd.sp_scale)>(head,"sp-scale");
        else
            grd.sp_scale = 1.0;
    }

    void processGlobalValues(DataGRD& grd, const TiXmlHandle& doc) {
        if (grd.simple)
            return;
            
        if (auto el = doc.FirstChildElement("globalcpoint").Element()) {
            grd.globalcp.set( numFromAttr<decltype(grd.globalcp.x)>(*el,"x",0,grd.cellwidth)  ,
                              numFromAttr<decltype(grd.globalcp.y)>(*el,"y",0,grd.cellheight));
        } else {
            grd.globalcp.set(0, 0);
        }

        fillAreasFromElement( doc.FirstChildElement("globalareas").FirstChildElement("area").ToElement(),
                              grd.globalareas                                                          );
    }

    void processSpriteValues(DataGRD& grd, const TiXmlHandle& doc) {
        for (auto pImg = doc.FirstChildElement("img").ToElement(); 
            pImg ; pImg = pImg->NextSiblingElement()) {
            
            DataGRD::Sprite spt; const auto& img = *pImg;
            spt.name = checkAttr(img,"name","",false) ? valFromAttr<std::string>(img,"name") : "noname" ;
            spt.dim.set( numFromAttr<decltype(spt.dim.x)>(img,"width",0,grd.cellwidth)   ,
                         numFromAttr<decltype(spt.dim.y)>(img,"height",0,grd.cellheight));

            spt.cp.set( grd.globalcp.x, grd.globalcp.y );
            if (auto el = img.FirstChildElement("cpoint")) {
                spt.cp.x += numFromAttr<decltype(spt.cp.x)>(*el,"x",0, spt.dim.x - grd.globalcp.x );
                spt.cp.y += numFromAttr<decltype(spt.cp.y)>(*el,"y",0, spt.dim.x - grd.globalcp.y );
            } else if (spt.cp.x > spt.dim.x || spt.cp.y > spt.dim.y)
                throw FSException("the global cp is not valid due to image sizes",__LINE__);
            
            fillAreasFromElement(img.FirstChildElement("area"), spt.areas);
        }
    }

    void createimages() {

    }

    void getimageattributes() {

    }

    void getcpoint() {

    }

    void createimage() {

    }

    void processareas() {

    }
};

#endif
