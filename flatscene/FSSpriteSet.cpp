#include "FSSpriteSet.h"
#include "FSLibrary.h"
 
#include <string>
#include <algorithm>

#include "debugfuncs.h"

Uint16 FSSpriteset::globalAuxiliar=0;

FSSpriteset::FSSpriteset()
{
	getSpriteList ( ).clear ( ) ;
	name="";
	this->mode = ONLY_TEXTURE;
	FSLibrary::Error("Por ahora no permito crear Spritesets de la nada.");
}

FSSpriteset::FSSpriteset(string c, Uint8 mode)
{
	globalAuxiliar=0;	// Lo ponemos a cero, pues vamos a empezar un ciclo.
	getSpriteList ( ).clear ( ) ;
	loadChipset(c,mode);
	this->mode = mode;
	name=c;
}

FSSpriteset::~FSSpriteset()
{

	SpriteCollection::iterator iter ;
	FSSprite* pspt ;
	while ( !getSpriteList ( ).empty ( ) )
	{
		iter = getSpriteList().begin ( ) ;
		pspt = *iter ;
		getSpriteList ( ).erase ( iter ) ;
		delete pspt ;
	}

	#ifdef LOG_SPRITESET
		printf("\nSpriteset %s deleted.\n",name.c_str());
	#endif
}

Uint8 FSSpriteset::getMode() {
	return mode;
}

string FSSpriteset::getName() {
	return name;
}

bool FSSpriteset::setName(string& name) {
	if (this->name=="") {
		this->name=name;
		return true;
	}
	return false;
}

void FSSpriteset::add ( FSSprite* pspt ) 
{
	getSpriteList ( ).push_back ( pspt ) ;
}

bool FSSpriteset::has ( FSSprite* pspt ) 
{
	SpriteCollection::iterator iter = find ( getSpriteList ( ).begin ( ) , getSpriteList ( ).end ( ) , pspt ) ;
	return ( iter != getSpriteList ( ).end ( ) ) ;
}

int FSSpriteset::search( FSSprite* pspt ) 
{
	SpriteCollection::iterator iter = find ( getSpriteList ( ).begin ( ) , getSpriteList ( ).end ( ) , pspt ) ;
	if ( iter == getSpriteList ( ).end ( ) ) return ( -1 ) ;
	return ( iter - getSpriteList ( ).begin ( ) ) ;
}

void FSSpriteset::remove ( FSSprite* pspt ) 
{
	SpriteCollection::iterator iter = find ( getSpriteList ( ).begin ( ) , getSpriteList ( ).end ( ) , pspt ) ;
	if ( iter == getSpriteList ( ).end ( ) ) return ;
	getSpriteList ( ).erase ( iter ) ;
}

FSSprite* FSSpriteset::get ( int n ) 
{
	if ( n < getSpriteList ().size()) {
		return ( getSpriteList ( ) [n] ) ;
	} else {
		return NULL;
	}
}

SpriteCollection& FSSpriteset::getSpriteList ( ) 
{
	return ( m_vecSprites ) ;
}

int FSSpriteset::size ( ) 
{
	return ( getSpriteList ( ).size ( ) ) ;
}

void FSSpriteset::loadChipset(string& c,Uint8 mode,string* cPrev) {

	int num_img=0;
	int ancho=0;
	int alto=0;

	int sp_scale = 1;

	SDL_Surface* chipset = NULL;

	bool simple = false;
	globalAuxiliar++;	// En cada recursión del método lo incrementamos.

	string s_aux=c;
	string tipefile("");
	string namefile("");


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

	if (cPrev) { // Es decir, si ha habido recursión debido a la función defined-in, caso especial.
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

	#ifdef LOG_SPRITESET
		if (cPrev == NULL)
			printf("\nSpriteset %s\n",c.c_str());
	#endif

	// Abrimos el .grd y reescatamos la estructura de datos.

	TiXmlDocument xmldoc(s_aux.c_str());
	if (!xmldoc.LoadFile()) {	 	// Cargamos el .grd
		// Si no carga, asumimos que no existe el .grd y el Spriteset será un único Sprite sin información adicional

		if (cPrev || tipefile == ".grd") // en ese caso no tiene sentido que haya varios ciclos
			 return FSLibrary::Error(s_aux,TE_fileExists);

		num_img = 1;

		s_aux=namefile+tipefile;

		chipset=IMG_Load(s_aux.c_str());
		if (!chipset) { FSLibrary::Error(s_aux.c_str(),TE_fileExists); }

		ancho = chipset->w;
		alto = chipset->h;

		if (SDL_GetVideoSurface() != NULL) {

			SDL_Surface* sdl_surf=SDL_DisplayFormat(chipset);
			if (sdl_surf) {
				SDL_FreeSurface(chipset);
				chipset = sdl_surf;
			} else {
				return FSLibrary::Error("SDL_DisplayFormat(chipset) fallo.");
			}

		} else if (mode != ONLY_SDL_SURFACE) 
			return FSLibrary::Error("Librería no inicializada antes de crear CSpriteSet con texturas. Usa ONLY_SDL_SURFACE.");

		FSSprite* pspt=new FSSprite(FSImage::toSCanvas(chipset,mode));

		pspt->setName(s_aux.c_str());

		add(pspt);

		return;

	} 

	if (cPrev)	
		c = *cPrev;	// Si este archivo ha sido enlazado desde otro .grd, sustituimos el nombre por si la recursión continua.

	if (globalAuxiliar > 100)
		return FSLibrary::Error(("Estructura defectuosa del archivo: "+c+". \nDependencias cíclicas.").c_str());	// Si la recursión es demasiado grande, ERROR.

	TiXmlHandle input(xmldoc.FirstChild());

	if (!input.ToElement()) { 
		return FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux).c_str()); 
	}

	if (input.ToElement()->Attribute("defined-in")) {
		string sdefined(input.ToElement()->Attribute("defined-in"));
		return loadChipset(sdefined,mode,&c);
	}

	if (!input.ToElement()->Attribute("sprites")) {	
		return FSLibrary::Error("Estructura defectuosa para el archivo: "+s_aux+"\nNo se ha especificado el número de sprites.");	
	}

	if (input.ToElement()->Attribute("type") && strcmp(input.ToElement()->Attribute("type"),"split")==0)
		return loadChipsetSplit(s_aux,mode);

	if (!(input.ToElement()->Attribute("cellwidth") && input.ToElement()->Attribute("cellheight"))) { 
		return FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nNo se ha especificado dimensiones para las celdas.").c_str()); 
	}

	input.Element()->QueryIntAttribute("sprites",&num_img);	// Tenemos el número de imágenes.
	input.Element()->QueryIntAttribute("cellwidth",&ancho);	// Tenemos el ancho de celda.
	input.Element()->QueryIntAttribute("cellheight",&alto);// Tenemos el alto de celda.


	if (input.ToElement()->Attribute("simple") && strcmp(input.ToElement()->Attribute("simple"),"true")==0)
		simple=true;	// Si el spriteset es de tipo simple, activamos este booleano.

	if (input.Element()->Attribute("sp-scale"))
		input.Element()->QueryIntAttribute("sp-scale",&sp_scale); // Hay escalado predefinido de SDL_Surface

	// Primer lote de la estructura de datos procesado
	// Ahora se empieza a carga la imagen con los datos obtenidos

	if (tipefile == ".png" || tipefile == ".bmp" || tipefile == ".jpg2") {
		s_aux=namefile + tipefile;

		chipset=IMG_Load(s_aux.c_str());
		if (!chipset) { 
			return FSLibrary::Error(s_aux,TE_fileExists); 
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
					return FSLibrary::Error(s_aux,TE_fileExists); 
				}
			}
		}
	} else {
		return FSLibrary::Error(s_aux + "No es un tipo de recurso grafico apropiado.",TE_standard);
	}

	SDL_Surface* sdl_surf = NULL;

	/*SDL_PixelFormat* f = chipset->format;
	printf("bpp:%d Amask:%X alpha:%d colorkey:%d Rmask:%X Gmask:%X Bmask:%X\n",f->BitsPerPixel,f->Amask,f->alpha,f->colorkey,f->Rmask,f->Gmask,f->Bmask);
	*/

	Uint32 columnas = chipset->w / ancho;		// Calculamos el número de columnas del spriteset.

	if (	(((float)columnas) - (((float)chipset->w)/((float)ancho))) != 0.00 || (		((float)(chipset->h / alto))	-  (((float)chipset->h)/((float)alto))		)!=0.00	)	{ 
		return FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nEl tamaño de la imagen debe ser múltiplo exacto de las celdas.").c_str()); 
	}

	if (columnas <= 0 || columnas > chipset->w) { 
		return FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nCantidad de columnas imposible.").c_str()); 
	}

	
	// Empezamos a preparar el formato de la imagen.

	

	if (SDL_GetVideoSurface() != NULL) {

		sdl_surf=SDL_DisplayFormatAlpha(chipset);
		if (sdl_surf) {
			SDL_FreeSurface(chipset);
			chipset=sdl_surf;
		} else {
			return FSLibrary::Error("SDL_DisplayFormat(chipset) fallo.");
		}

	} else if (mode != ONLY_SDL_SURFACE) 
		return FSLibrary::Error("Librería no inicializada antes de crear CSpriteSet con texturas. Usa ONLY_SDL_SURFACE.");
	
	s_aux.clear();

	SDL_Rect rect; // Este rectangulo llevara la cuenta de las coordenadas por la que nos encontramos dentro del spriteset (chipset).
	rect.x=0;
	rect.y=0;

	SCanvas m_pImage;
	FSSprite* m_pSprite;

	struct { int w,h,nump; const char *name; } iinfo;

	TiXmlElement* img=NULL;

	FSPoint globalCP;	// El punto central global es opcional, y si no se encuentra, se le asigna -1 a X.
	globalCP.setX(-1);
	map<int,RectsInfo> globalAreas;

	s_aux = namefile + ".grd";

	// Otro lote de la estructura de datos necesario para recortar la imagen.

	if (!simple) {		// Si el spriteset no es simple...
		img = input.FirstChildElement("globalcpoint").ToElement();
		if (img) {	// Si hay punto central global...
			if (!(img->Attribute("x") && img->Attribute("y") && 
				atoi(img->Attribute("x"))>=0 && atoi(img->Attribute("x"))<=ancho && 
				atoi(img->Attribute("y"))>=0 && atoi(img->Attribute("y"))<=alto)) 
					FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores de punto centro global erróneos.").c_str());
			globalCP.set(atoi(img->Attribute("x"))*sp_scale,atoi(img->Attribute("y"))*sp_scale);	// Lo asignamos.
		}

		img = input.FirstChildElement("globalareas").FirstChildElement("area").ToElement();
		while (img) {	// Mientras haya areas globales...
			if (!img->Attribute("id") || atoi(img->Attribute("id"))<0)
				FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores de areas globales erróneos.").c_str());
			int idArea = atoi(img->Attribute("id")); // Determinamos que número de area es (las areas van ordenadas segun el conjunto de los numeros naturales, de forma continua y orden ascendente).

			if (img->Attribute("relative") && strcmp(img->Attribute("relative"),"true")==0)	// Miramos si las coordenadas de esa area estan descritas en terminos relativos (segun el punto de centro) o absolutos.
				globalAreas[idArea].relative=true;	// Y la añadimos a la posicion del mapa correspondiente
			else
				globalAreas[idArea].relative=false;

			for (TiXmlElement* rectNode=img->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement("rectangle")) {		// Por cada rectangulo de esa area...
				if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2"))) 
					FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rectángulo un área global erróneos.").c_str());

				SDL_Rect_Signed rc;
				rc.x = atoi(rectNode->Attribute("x1")) *sp_scale;
				rc.w = atoi(rectNode->Attribute("x2")) *sp_scale;
				rc.y = atoi(rectNode->Attribute("y1")) *sp_scale;
				rc.h = atoi(rectNode->Attribute("y2")) *sp_scale;

				globalAreas[idArea].v.push_back(rc);	// Y la añadimos al vector pertinente del mapa, para luego reescatarla directamente del mapa.
			}

			img = img->NextSiblingElement("area");
		}

		img = input.FirstChildElement("img").ToElement();		// Preparamos el puntero al siguiente nodo imagen.
	} else {
		#ifdef LOG_SPRITESET_INFO
			printf("%d Simple Sprites\n",num_img);
		#endif
	}

	// Recorte de cada fragmento de la imagen

	for (int i=0;img || ( simple && (i<num_img) );i++) {	// Mientras haya un nodo imagen siguiente o si el spriteset es simple y segun la cuenta aun no hemos llegado a la última imagen...
		if (!simple && img->Attribute("name")) {	// Asignamos nombre si tiene, si no "noname".
			iinfo.name = img->Attribute("name");
		} else {
			iinfo.name = "noname";
		}
		#ifdef LOG_SPRITESET_INFO
			if (!simple) printf("Image%d %s\n",i,iinfo.name);
		#endif
		if (!simple && img->Attribute("width")) {	// Asignamos al rectangulo de coordenas la anchura. Si no tiene especifica, no por defecto es el ancho de celda.
			iinfo.w = atoi(img->Attribute("width"));
			if (iinfo.w > ancho || iinfo.w <= 0)		{	FSLibrary::Error("Estructura defectuosa del archivo: "+s_aux+"\nValores en la imagen "+iinfo.name+" erróneos.");	}
		}	else
			iinfo.w = ancho;
		if (!simple && img->Attribute("height")) { // Asignamos al rectangulo de coordenas la altura. Si no tiene especifica, no por defecto es el alto de celda.
			iinfo.h =	atoi(img->Attribute("height"));
			if (iinfo.h > alto || iinfo.h <= 0)	{	FSLibrary::Error("Estructura defectuosa del archivo: "+s_aux+"\nValores en la imagen "+iinfo.name+" erróneos.");	}
		}	else
			iinfo.h = alto;

		if (((i%columnas)==0) && (i!=0)) { // Si pasamos de la última columna, volvemos a la columna 0 y avanzamos 1 fila.
			rect.x=0;
			rect.y+=alto;
			
		} 
		// Desplazamos las coordenadas hacia la siguiente celda.
		rect.h=rect.y+iinfo.h;	
		rect.w=rect.x+iinfo.w;

		SDL_Surface* temp;

		sdl_surf = SDL_CreateRGBSurface(chipset->flags | SDL_SRCALPHA,iinfo.w,iinfo.h,chipset->format->BitsPerPixel,chipset->format->Rmask,chipset->format->Gmask,chipset->format->Bmask,chipset->format->Amask);	// Creamos una surface que contendra la informacion de la celda actual.
		//sdl_surf = SDL_CreateRGBSurface(chipset->flags | SDL_SRCALPHA,iinfo.w,iinfo.h,chipset->format->BitsPerPixel,0xFF,0xFF00,0xFF0000,0xFF000000);	// Creamos una surface que contendra la informacion de la celda actual.
		//sdl_surf = SDL_CreateRGBSurface(chipset->flags | SDL_SRCALPHA,iinfo.w,iinfo.h,chipset->format->BitsPerPixel,0,0,0,0);	// Creamos una surface que contendra la informacion de la celda actual.

		SDL_SetColorKey(sdl_surf,SDL_SRCCOLORKEY, chipset->format->colorkey);	// Le asignamos los formatos de la surface del spriteset.

		//SDL_BlitSurface(chipset,&rect,sdl_surf,0);

		blitcopy(chipset,&rect,sdl_surf,NULL);

		//countalpha(sdl_surf);

		
		if (sp_scale > 0 && sp_scale != 1.0 && mode != ONLY_SDL_SURFACE) {
			temp = FSImage::scaleSurface(sdl_surf,sp_scale);
			SDL_FreeSurface(sdl_surf);
			sdl_surf=temp;
			SDL_SetColorKey(sdl_surf,SDL_SRCCOLORKEY,chipset->format->colorkey);		// Reasignamos los formatos.
		}

		m_pImage=FSImage::toSCanvas(sdl_surf,mode);		// Convertimos la SDL_Surface en SCanvas(

		//Imagen creada, ahora el resto de su estructura de datos.

		TiXmlElement* cpoint=NULL; 
		if (!simple && (globalCP.X()<0)) // Y ahora si el spriteset no es simple y no existe globalcpoint, intentamos rescatar el cpoint. 
			cpoint = input.ChildElement("img",i).FirstChildElement("cpoint").ToElement();

		if (cpoint) {	// Si es procedente rescatar el cpoint...
			#ifdef LOG_SPRITESET_INFO
				printf("\tCP : %d,%d\n",atoi(cpoint->Attribute("x"))*sp_scale,atoi(cpoint->Attribute("y"))*sp_scale);
			#endif
			if (!(cpoint->Attribute("x") && cpoint->Attribute("y") && 
				atoi(cpoint->Attribute("x"))>=0 && atoi(cpoint->Attribute("x"))<=iinfo.w && 
				atoi(cpoint->Attribute("y"))>=0 && atoi(cpoint->Attribute("y"))<=iinfo.h)) 
					FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en el punto centro de la imagen "+iinfo.name+" erróneos.").c_str());
			m_pSprite=new FSSprite(m_pImage,new FSPoint(atoi(cpoint->Attribute("x"))*sp_scale,atoi(cpoint->Attribute("y"))*sp_scale));
		} else if (globalCP.X()>=0) { // Si existe un global cpoint...
			m_pSprite=new FSSprite(m_pImage,new FSPoint(globalCP));
		} else {	
			m_pSprite=new FSSprite(m_pImage);	// Acabamos llamando al constructor del sprite, con o sin cpoint.
		}

		TiXmlElement* area = NULL;
		if (!simple)	
			area = input.ChildElement("img",i).FirstChildElement("area").ToElement(); // Si el spriteset no es simple, buscamos areas en la imagen.

		for (int numArea=0,countGlobalAreas=0;(countGlobalAreas < globalAreas.size()) || (area && area->Attribute("id"));numArea++) {	// Si hay...
			RectArea* rArea = new RectArea; // Preparamos la nueva area.
		#ifdef LOG_SPRITESET_INFO
			int idebug=0;
		#endif
			if (globalAreas.find(numArea)==globalAreas.end()) { // Si esta area no se corresponde con ninguna de las areas globales...
				if (!area)	// Si no existe area, es que las areas globales estan mal definidas en el fichero .grd
					FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nSe requiere un orden ascendente coherente entre areas (sin saltos) teniendo en cuenta las areas globales.").c_str());
				bool relative = area->Attribute("relative") && strcmp(area->Attribute("relative"),"true")==0;	// Rescatamos si las coordenadas son relativas.
				for (TiXmlElement* rectNode=area->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement("rectangle")) { // Por cada rectangulo perteneciente al area.
					if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2"))) 
						FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rectángulo de la imagen "+iinfo.name+" erróneos.").c_str());

					SDL_Rect_Signed rc;
					rc.x = atoi(rectNode->Attribute("x1")); // Anotamos las coordenadas.
					rc.w = atoi(rectNode->Attribute("x2"));
					rc.y = atoi(rectNode->Attribute("y1"));
					rc.h = atoi(rectNode->Attribute("y2"));
					if (!relative) {	// Si son relativas, tenemos en cuentra el punto de centro.
						rc.x -= m_pSprite->getCenter()->getX();
						rc.w -= m_pSprite->getCenter()->getX();
						rc.y -= m_pSprite->getCenter()->getY();
						rc.h -= m_pSprite->getCenter()->getY();
					}

					if (/*(rc.x < 0) || (rc.x > iinfo.w) || (rc.y < 0) || (rc.y > iinfo.w) || (rc.w < 0) || (rc.w > iinfo.h) || (rc.h < 0) || (rc.h > iinfo.h) || */(rc.w<rc.x) || (rc.h<rc.y))
						FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rectángulo de la imagen "+iinfo.name+" conflictivos.").c_str());

					rArea->push_back(new FSRectangle(rc.x *sp_scale,rc.y *sp_scale,rc.w *sp_scale,rc.h *sp_scale));	// Finalmente creamos con ellas un rectangulo y lo añadimos al area actual.
				#ifdef LOG_SPRITESET_INFO
					printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x*sp_scale,rc.w*sp_scale,rc.y*sp_scale,rc.h*sp_scale);
					idebug++;
				#endif
				}
				area=area->NextSiblingElement("area");
			} else {		//Si es un area global.
				for (vector<SDL_Rect_Signed>::iterator it=globalAreas[numArea].v.begin();it!=globalAreas[numArea].v.end();++it) {
					SDL_Rect_Signed rc = *it;	// Reescatamos la informacion de las coordenadas anotada anteriormente y guardado en el vector perteneciente a el pertinente puesto del mapa.
					if (!globalAreas[numArea].relative) {	// Si son relativas, tenemos en cuenta el punto de centro.
						rc.x -= m_pSprite->getCenter()->getX();
						rc.w -= m_pSprite->getCenter()->getX();
						rc.y -= m_pSprite->getCenter()->getY();
						rc.h -= m_pSprite->getCenter()->getY();
					}
					if (/*(rc.x < 0) || (rc.x > iinfo.w) || (rc.y < 0) || (rc.y > iinfo.w) || (rc.w < 0) || (rc.w > iinfo.h) || (rc.h < 0) || (rc.h > iinfo.h) ||*/ (rc.w<rc.x) || (rc.h<rc.y))
						FSLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rectángulo global aplicado la imagen "+iinfo.name+" conflictivos.").c_str());

					rArea->push_back(new FSRectangle(rc.x*sp_scale,rc.y*sp_scale,rc.w*sp_scale,rc.h*sp_scale)); // Finalmente creamos con ellas un rectangulo y lo añadimos al area actual.
				#ifdef LOG_SPRITESET_INFO
					printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x*sp_scale,rc.w*sp_scale,rc.y*sp_scale,rc.h*sp_scale);
					idebug++;
				#endif
				}
				countGlobalAreas++;
			}
			m_pSprite->addArea(rArea);	// Finalmente añadimos el area a este sprite.
		}


		m_pSprite->setName(iinfo.name);	// Asignamos el nombre al sprite reciencreado.

		add(m_pSprite);	//Añadimos el sprite a este objeto cspriteset.

		rect.x+=ancho;	// Aumentamos la coordenada hacia la siguiente columna.

		if (!simple)
			img=img->NextSiblingElement("img");	// Si no es simple, buscamos la siguiente etiqueta imagen.
	}
	SDL_FreeSurface(chipset);
	chipset=NULL;

}

void FSSpriteset::loadChipsetSplit(string grd,Uint8 mode) {

	int num_img=0;
	int ancho=0;
	int alto=0;

	int sp_scale = 1;

	string folder = grd;

	for (int i=folder.size()-1;i>=0;i--) {
		char letra = folder.at(i);

		folder = folder.substr(0,folder.size()-1);

		if (letra=='/' || letra=='\\')
			break;			
	}

	//if (folder.empty())
	//	folder = ".";

	TiXmlDocument grddoc(grd.c_str());

	if (!grddoc.LoadFile())
		FSLibrary::Error(grd.c_str(),TE_fileExists);

	TiXmlElement* head = grddoc.FirstChildElement("Spriteset");

	if (!head || !head->Attribute("type") || strcmp(head->Attribute("type"),"split")!=0)
		return FSLibrary::Error("Estructura defectuosa para el archivo: "+grd+". Debería tratarse de un 'grd' de tipo 'split' formal.");

	head->QueryIntAttribute("sprites",&num_img);

	if (head->Attribute("sp-scale"))
		head->QueryIntAttribute("sp-scale",&sp_scale); // Hay escalado predefinido de SDL_Surface


	FSPoint globalCP;	// El punto central global es opcional, y si no se encuentra, se le asigna -1 a X.
	globalCP.setX(-1);
	map<int,RectsInfo> globalAreas;

	TiXmlHandle input = grddoc.FirstChild();

	TiXmlElement* elmnt = input.FirstChildElement("globalcpoint").ToElement();

	if (elmnt) {	// Si hay punto central global...
		if (!(elmnt->Attribute("x") && elmnt->Attribute("y") && 
			atoi(elmnt->Attribute("x"))>=0 && atoi(elmnt->Attribute("x"))<=ancho && 
			atoi(elmnt->Attribute("y"))>=0 && atoi(elmnt->Attribute("y"))<=alto)) 
				FSLibrary::Error(("Estructura defectuosa del archivo: "+grd+"\nValores de punto centro global erróneos.").c_str());
		globalCP.set(atoi(elmnt->Attribute("x"))*sp_scale,atoi(elmnt->Attribute("y"))*sp_scale);	// Lo asignamos.
	}

	elmnt = input.FirstChildElement("globalareas").FirstChildElement("area").ToElement();
	while (elmnt) {	// Mientras haya areas globales...
		if (!elmnt->Attribute("id") || atoi(elmnt->Attribute("id"))<0)
			FSLibrary::Error(("Estructura defectuosa del archivo: "+grd+"\nValores de areas globales erróneos.").c_str());
		int idArea = atoi(elmnt->Attribute("id")); // Determinamos que número de area es (las areas van ordenadas segun el conjunto de los numeros naturales, de forma continua y orden ascendente).

		if (elmnt->Attribute("relative") && strcmp(elmnt->Attribute("relative"),"true")==0)	// Miramos si las coordenadas de esa area estan descritas en terminos relativos (segun el punto de centro) o absolutos.
			globalAreas[idArea].relative=true;	// Y la añadimos a la posicion del mapa correspondiente
		else
			globalAreas[idArea].relative=false;

		for (TiXmlElement* rectNode=elmnt->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement("rectangle")) {		// Por cada rectangulo de esa area...
			if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2"))) 
				FSLibrary::Error(("Estructura defectuosa del archivo: "+grd+"\nValores en un rectángulo un área global erróneos.").c_str());

			SDL_Rect_Signed rc;
			rc.x = atoi(rectNode->Attribute("x1")) *sp_scale;
			rc.w = atoi(rectNode->Attribute("x2")) *sp_scale;
			rc.y = atoi(rectNode->Attribute("y1")) *sp_scale;
			rc.h = atoi(rectNode->Attribute("y2")) *sp_scale;

			globalAreas[idArea].v.push_back(rc);	// Y la añadimos al vector pertinente del mapa, para luego reescatarla directamente del mapa.
		}

		elmnt = elmnt->NextSiblingElement("area");

	}

	SDL_Rect rect; // Este rectangulo llevara la cuenta de las coordenadas por la que nos encontramos dentro del spriteset (chipset).
	rect.x=0;
	rect.y=0;

	SCanvas m_pImage;
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
						return FSLibrary::Error("SDL_DisplayFormat(chipset) fallo.");
					}

				} else if (mode != ONLY_SDL_SURFACE) 
					return FSLibrary::Error("Librería no inicializada antes de crear CSpriteSet con texturas. Usa ONLY_SDL_SURFACE.");

				if (sp_scale > 0 && sp_scale != 1.0 && mode != ONLY_SDL_SURFACE) {
					temp = FSImage::scaleSurface(sdl_surf,sp_scale);
					SDL_FreeSurface(sdl_surf);
					sdl_surf=temp;
					SDL_SetColorKey(sdl_surf,SDL_SRCCOLORKEY,sdl_surf->format->colorkey);		// Reasignamos los formatos.
				}



				m_pImage = FSImage::toSCanvas(sdl_surf,mode);

				TiXmlElement* cpoint = NULL;

				if (globalCP.X() < 0)
					cpoint = elmnt->FirstChildElement("cpoint");

				if (cpoint) {
					if (!(cpoint->Attribute("x") && cpoint->Attribute("y") && 
						atoi(cpoint->Attribute("x"))>=0 && atoi(cpoint->Attribute("x"))<=m_pImage.w2 && 
						atoi(cpoint->Attribute("y"))>=0 && atoi(cpoint->Attribute("y"))<=m_pImage.h2)) 
							FSLibrary::Error(("Estructura defectuosa del archivo: "+grd+"\nValores en el punto centro de la imagen "+iinfo.name+" erróneos.").c_str());
					m_pSprite=new FSSprite(m_pImage,new FSPoint(atoi(cpoint->Attribute("x"))*sp_scale,atoi(cpoint->Attribute("y"))*sp_scale));
				} else if (globalCP.X()>=0) { // Si existe un global cpoint...
					m_pSprite=new FSSprite(m_pImage,new FSPoint(globalCP));
				} else {	
					m_pSprite=new FSSprite(m_pImage);	// Acabamos llamando al constructor del sprite, con o sin cpoint.
				}

			}

		}


		TiXmlElement* area = elmnt->FirstChildElement("area"); // Si el spriteset no es simple, buscamos areas en la imagen.

		for (int numArea=0,countGlobalAreas=0;(countGlobalAreas < globalAreas.size()) || (area && area->Attribute("id"));numArea++) {	// Si hay...
			RectArea* rArea = new RectArea; // Preparamos la nueva area.
		#ifdef LOG_SPRITESET_INFO
			int idebug=0;
		#endif
			if (globalAreas.find(numArea)==globalAreas.end()) { // Si esta area no se corresponde con ninguna de las areas globales...
				if (!area)	// Si no existe area, es que las areas globales estan mal definidas en el fichero .grd
					FSLibrary::Error(("Estructura defectuosa del archivo: "+grd+"\nSe requiere un orden ascendente coherente entre areas (sin saltos) teniendo en cuenta las areas globales.").c_str());
				bool relative = area->Attribute("relative") && strcmp(area->Attribute("relative"),"true")==0;	// Rescatamos si las coordenadas son relativas.
				for (TiXmlElement* rectNode=area->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement("rectangle")) { // Por cada rectangulo perteneciente al area.
					if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2"))) 
						FSLibrary::Error(("Estructura defectuosa del archivo: "+grd+"\nValores en un rectángulo de la imagen "+iinfo.name+" erróneos.").c_str());

					SDL_Rect_Signed rc;
					rc.x = atoi(rectNode->Attribute("x1")); // Anotamos las coordenadas.
					rc.w = atoi(rectNode->Attribute("x2"));
					rc.y = atoi(rectNode->Attribute("y1"));
					rc.h = atoi(rectNode->Attribute("y2"));
					if (!relative) {	// Si son relativas, tenemos en cuentra el punto de centro.
						rc.x -= m_pSprite->getCenter()->getX();
						rc.w -= m_pSprite->getCenter()->getX();
						rc.y -= m_pSprite->getCenter()->getY();
						rc.h -= m_pSprite->getCenter()->getY();
					}

					if (/*(rc.x < 0) || (rc.x > iinfo.w) || (rc.y < 0) || (rc.y > iinfo.w) || (rc.w < 0) || (rc.w > iinfo.h) || (rc.h < 0) || (rc.h > iinfo.h) || */(rc.w<rc.x) || (rc.h<rc.y))
						FSLibrary::Error(("Estructura defectuosa del archivo: "+grd+"\nValores en un rectángulo de la imagen "+iinfo.name+" conflictivos.").c_str());

					rArea->push_back(new FSRectangle(rc.x *sp_scale,rc.y *sp_scale,rc.w *sp_scale,rc.h *sp_scale));	// Finalmente creamos con ellas un rectangulo y lo añadimos al area actual.
				#ifdef LOG_SPRITESET_INFO
					printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x*sp_scale,rc.w*sp_scale,rc.y*sp_scale,rc.h*sp_scale);
					idebug++;
				#endif
				}
				area=area->NextSiblingElement("area");
			} else {		//Si es un area global.
				for (vector<SDL_Rect_Signed>::iterator it=globalAreas[numArea].v.begin();it!=globalAreas[numArea].v.end();++it) {
					SDL_Rect_Signed rc = *it;	// Reescatamos la informacion de las coordenadas anotada anteriormente y guardado en el vector perteneciente a el pertinente puesto del mapa.
					if (!globalAreas[numArea].relative) {	// Si son relativas, tenemos en cuenta el punto de centro.
						rc.x -= m_pSprite->getCenter()->getX();
						rc.w -= m_pSprite->getCenter()->getX();
						rc.y -= m_pSprite->getCenter()->getY();
						rc.h -= m_pSprite->getCenter()->getY();
					}
					if (/*(rc.x < 0) || (rc.x > iinfo.w) || (rc.y < 0) || (rc.y > iinfo.w) || (rc.w < 0) || (rc.w > iinfo.h) || (rc.h < 0) || (rc.h > iinfo.h) ||*/ (rc.w<rc.x) || (rc.h<rc.y))
						FSLibrary::Error(("Estructura defectuosa del archivo: "+grd+"\nValores en un rectángulo global aplicado la imagen "+iinfo.name+" conflictivos.").c_str());

					rArea->push_back(new FSRectangle(rc.x*sp_scale,rc.y*sp_scale,rc.w*sp_scale,rc.h*sp_scale)); // Finalmente creamos con ellas un rectangulo y lo añadimos al area actual.
				#ifdef LOG_SPRITESET_INFO
					printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x*sp_scale,rc.w*sp_scale,rc.y*sp_scale,rc.h*sp_scale);
					idebug++;
				#endif
				}
				countGlobalAreas++;
			}
			m_pSprite->addArea(rArea);	// Finalmente añadimos el area a este sprite.
		}


		m_pSprite->setName(iinfo.name);	// Asignamos el nombre al sprite reciencreado.

		add(m_pSprite);	//Añadimos el sprite a este objeto cspriteset.

		elmnt = elmnt->NextSiblingElement("img");	// Si no es simple, buscamos la siguiente etiqueta imagen.
	}


}
/*
void CSpriteset::operator delete(void* m) {
	delete ((CSpriteset*)m);
}*/
