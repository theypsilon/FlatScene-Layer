#include "FSSpriteSet.h"
#include "FSLibrary.h"
 
#include <string>
#include <algorithm>

Uint16 CSpriteset::globalAuxiliar=0;

CSpriteset::CSpriteset()
{
	getSpriteList ( ).clear ( ) ;
	name="";
	this->mode = ONLY_TEXTURE;
	CLibrary::Error("Por ahora no permito crear Spritesets de la nada.");
}

CSpriteset::CSpriteset(string c, Uint8 mode)
{
	globalAuxiliar=0;	// Lo ponemos a cero, pues vamos a empezar un ciclo.
	getSpriteList ( ).clear ( ) ;
	loadChipset(c,mode);
	this->mode = mode;
	name=c;
}

CSpriteset::~CSpriteset()
{

	SpriteCollection::iterator iter ;
	CSprite* pspt ;
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

Uint8 CSpriteset::getMode() {
	return mode;
}

string CSpriteset::getName() {
	return name;
}

bool CSpriteset::setName(string& name) {
	if (this->name=="") {
		this->name=name;
		return true;
	}
	return false;
}

void CSpriteset::add ( CSprite* pspt ) 
{
	getSpriteList ( ).push_back ( pspt ) ;
}

bool CSpriteset::has ( CSprite* pspt ) 
{
	SpriteCollection::iterator iter = find ( getSpriteList ( ).begin ( ) , getSpriteList ( ).end ( ) , pspt ) ;
	return ( iter != getSpriteList ( ).end ( ) ) ;
}

int CSpriteset::search( CSprite* pspt ) 
{
	SpriteCollection::iterator iter = find ( getSpriteList ( ).begin ( ) , getSpriteList ( ).end ( ) , pspt ) ;
	if ( iter == getSpriteList ( ).end ( ) ) return ( -1 ) ;
	return ( iter - getSpriteList ( ).begin ( ) ) ;
}

void CSpriteset::remove ( CSprite* pspt ) 
{
	SpriteCollection::iterator iter = find ( getSpriteList ( ).begin ( ) , getSpriteList ( ).end ( ) , pspt ) ;
	if ( iter == getSpriteList ( ).end ( ) ) return ;
	getSpriteList ( ).erase ( iter ) ;
}

CSprite* CSpriteset::get ( int n ) 
{
	if ( n < getSpriteList ().size()) {
		return ( getSpriteList ( ) [n] ) ;
	} else {
		return NULL;
	}
}

SpriteCollection& CSpriteset::getSpriteList ( ) 
{
	return ( m_vecSprites ) ;
}

int CSpriteset::size ( ) 
{
	return ( getSpriteList ( ).size ( ) ) ;
}

void CSpriteset::loadChipset(string& c,Uint8 mode,string* cPrev) {
	int num_img=0;
	int ancho=0;
	int alto=0;

	float sp_scale = 1.0;

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
			tipefile = ".png";
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

	TiXmlDocument xmldoc(resource(s_aux.c_str()).c_str());
	if (!xmldoc.LoadFile()) {	 	// Cargamos el .grd
		// Si no carga, asumimos que no existe el .grd y el Spriteset será un único Sprite sin información adicional

		if (cPrev) // en ese caso no tiene sentido que haya varios ciclos
			 return CLibrary::Error(s_aux,TE_fileExists);

		num_img = 1;

		s_aux=namefile+tipefile;

		chipset=IMG_Load(resource(s_aux.c_str()).c_str());
		if (!chipset) { CLibrary::Error(s_aux,TE_fileExists); }

		ancho = chipset->w;
		alto = chipset->h;

		SDL_Surface* sdl_surf=SDL_DisplayFormat(chipset);
		if (sdl_surf) {
			SDL_FreeSurface(chipset);
			chipset = sdl_surf;
		} else {
			return CLibrary::Error("SDL_DisplayFormat(chipset) fallo.");
		}

		if (sp_scale > 0 && sp_scale != 1.0) {

			Uint32 colorkey = sdl_surf->format->colorkey;

			chipset = CImage::scaleSurface(sdl_surf,sp_scale);
			SDL_FreeSurface(sdl_surf);
			sdl_surf=NULL;
			SDL_SetColorKey(chipset,SDL_SRCCOLORKEY,colorkey);		// Reasignamos los formatos.

		}

		CSprite* pspt=new CSprite(CImage::toSCanvas(chipset,mode,sp_scale));

		pspt->setName(s_aux.c_str());

		add(pspt);

		return;

	} 

	if (cPrev)	
		c = *cPrev;	// Si este archivo ha sido enlazado desde otro .grd, sustituimos el nombre por si la recursión continua.

	if (globalAuxiliar > 100)
		return CLibrary::Error(("Estructura defectuosa del archivo: "+c+". \nDependencias cíclicas.").c_str());	// Si la recursión es demasiado grande, ERROR.

	TiXmlHandle input(xmldoc.FirstChild());

	if (!input.ToElement()) { 
		return CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux).c_str()); 
	}

	if (input.ToElement()->Attribute("defined-in")) {
		string sdefined(input.ToElement()->Attribute("defined-in"));
		return loadChipset(sdefined,mode,&c);
	}

	if (!input.ToElement()->Attribute("sprites")) {	
		return CLibrary::Error("Estructura defectuosa para el archivo: "+s_aux+"\nNo se ha especificado el número de sprites.");	
	}

	if (!(input.ToElement()->Attribute("cellwidth") && input.ToElement()->Attribute("cellheight"))) { 
		return CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nNo se ha especificado dimensiones para las celdas.").c_str()); 
	}

	input.Element()->QueryIntAttribute("sprites",&num_img);	// Tenemos el número de imágenes.
	input.Element()->QueryIntAttribute("cellwidth",&ancho);	// Tenemos el ancho de celda.
	input.Element()->QueryIntAttribute("cellheight",&alto);// Tenemos el alto de celda.


	if (input.ToElement()->Attribute("simple") && strcmp(input.ToElement()->Attribute("simple"),"true")==0)
		simple=true;	// Si el spriteset es de tipo simple, activamos este booleano.

	if (input.Element()->Attribute("sp-scale"))
		input.Element()->QueryFloatAttribute("sp-scale",&sp_scale); // Hay escalado predefinido de SDL_Surface

	// Primer lote de la estructura de datos procesado
	// Ahora se empieza a carga la imagen con los datos obtenidos

	s_aux=namefile + tipefile;

	chipset=IMG_Load(resource(s_aux.c_str()).c_str());
	if (!chipset) { 
		return CLibrary::Error(s_aux,TE_fileExists); 
	}

	Uint32 columnas = chipset->w / ancho;		// Calculamos el número de columnas del spriteset.

	if (	(((float)columnas) - (((float)chipset->w)/((float)ancho))) != 0.00 || (		((float)(chipset->h / alto))	-  (((float)chipset->h)/((float)alto))		)!=0.00	)	{ 
		return CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nEl tamaño de la imagen debe ser múltiplo exacto de las celdas.").c_str()); 
	}

	if (columnas <= 0 || columnas > chipset->w) { 
		return CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nCantidad de columnas imposible.").c_str()); 
	}

	
	// Empezamos a preparar el formato de la imagen.

	SDL_Surface* sdl_surf;

	if (mode != ONLY_SDL_SURFACE) {
		sdl_surf=SDL_DisplayFormatAlpha(chipset);
		if (sdl_surf) {
			SDL_FreeSurface(chipset);
			chipset=sdl_surf;
		} else {
			return CLibrary::Error("SDL_DisplayFormat(chipset) fallo.");
		}
	}
	
	s_aux.clear();

	SDL_Rect rect; // Este rectangulo llevara la cuenta de las coordenadas por la que nos encontramos dentro del spriteset (chipset).
	rect.x=0;
	rect.y=0;

	SCanvas m_pImage;
	CSprite* m_pSprite;

	struct { int w,h,nump; const char *name; } iinfo;

	TiXmlElement* img=NULL;

	CPoint globalCP;	// El punto central global es opcional, y si no se encuentra, se le asigna -1 a X.
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
					CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores de punto centro global erróneos.").c_str());
			globalCP.set(atoi(img->Attribute("x")),atoi(img->Attribute("y")));	// Lo asignamos.
		}

		img = input.FirstChildElement("globalareas").FirstChildElement("area").ToElement();
		while (img) {	// Mientras haya areas globales...
			if (!img->Attribute("id") || atoi(img->Attribute("id"))<0)
				CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores de areas globales erróneos.").c_str());
			int idArea = atoi(img->Attribute("id")); // Determinamos que número de area es (las areas van ordenadas segun el conjunto de los numeros naturales, de forma continua y orden ascendente).

			if (img->Attribute("relative") && strcmp(img->Attribute("relative"),"true")==0)	// Miramos si las coordenadas de esa area estan descritas en terminos relativos (segun el punto de centro) o absolutos.
				globalAreas[idArea].relative=true;	// Y la añadimos a la posicion del mapa correspondiente
			else
				globalAreas[idArea].relative=false;

			for (TiXmlElement* rectNode=img->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement()) {		// Por cada rectangulo de esa area...
				if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2"))) 
					CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rectángulo un área global erróneos.").c_str());

				SDL_Rect_Signed rc;
				rc.x = atoi(rectNode->Attribute("x1"));
				rc.w = atoi(rectNode->Attribute("x2"));
				rc.y = atoi(rectNode->Attribute("y1"));
				rc.h = atoi(rectNode->Attribute("y2"));

				globalAreas[idArea].v.push_back(rc);	// Y la añadimos al vector pertinente del mapa, para luego reescatarla directamente del mapa.
			}

			img = img->NextSiblingElement();
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
			if (iinfo.w > ancho || iinfo.w <= 0)		{	CLibrary::Error("Estructura defectuosa del archivo: "+s_aux+"\nValores en la imagen "+iinfo.name+" erróneos.");	}
		}	else
			iinfo.w = ancho;
		if (!simple && img->Attribute("height")) { // Asignamos al rectangulo de coordenas la altura. Si no tiene especifica, no por defecto es el alto de celda.
			iinfo.h =	atoi(img->Attribute("height"));
			if (iinfo.h > alto || iinfo.h <= 0)	{	CLibrary::Error("Estructura defectuosa del archivo: "+s_aux+"\nValores en la imagen "+iinfo.name+" erróneos.");	}
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

		sdl_surf = SDL_CreateRGBSurface(SDL_SRCALPHA,iinfo.w,iinfo.h,chipset->format->BitsPerPixel,0,0,0,0);	// Creamos una surface que contendra la informacion de la celda actual.

		SDL_SetColorKey(sdl_surf,SDL_SRCCOLORKEY, chipset->format->colorkey);	// Le asignamos los formatos de la surface del spriteset.
		SDL_BlitSurface(chipset,&rect,sdl_surf,0);

		if (sp_scale > 0 && sp_scale != 1.0) {
			temp = CImage::scaleSurface(sdl_surf,sp_scale);
			SDL_FreeSurface(sdl_surf);
			sdl_surf=temp;
			SDL_SetColorKey(sdl_surf,SDL_SRCCOLORKEY,chipset->format->colorkey);		// Reasignamos los formatos.
		}

		m_pImage=CImage::toSCanvas(sdl_surf,mode,sp_scale);		// Convertimos la SDL_Surface en SCanvas

		//Imagen creada, ahora el resto de su estructura de datos.

		TiXmlElement* cpoint=NULL; 
		if (!simple && (globalCP.X()<0)) // Y ahora si el spriteset no es simple y no existe globalcpoint, intentamos rescatar el cpoint. 
			cpoint = input.ChildElement("img",i).FirstChildElement("cpoint").ToElement();

		if (cpoint) {	// Si es procedente rescatar el cpoint...
			#ifdef LOG_SPRITESET_INFO
				printf("\tCP : %d,%d\n",atoi(cpoint->Attribute("x")),atoi(cpoint->Attribute("y")));
			#endif
			if (!(cpoint->Attribute("x") && cpoint->Attribute("y") && 
				atoi(cpoint->Attribute("x"))>=0 && atoi(cpoint->Attribute("x"))<=iinfo.w && 
				atoi(cpoint->Attribute("y"))>=0 && atoi(cpoint->Attribute("y"))<=iinfo.h)) 
					CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en el punto centro de la imagen "+iinfo.name+" erróneos.").c_str());
			m_pSprite=new CSprite(m_pImage,new CPoint(atoi(cpoint->Attribute("x")),atoi(cpoint->Attribute("y"))));
		} else if (globalCP.X()>=0) { // Si existe un global cpoint...
			m_pSprite=new CSprite(m_pImage,new CPoint(globalCP));
		} else {	
			m_pSprite=new CSprite(m_pImage);	// Acabamos llamando al constructor del sprite, con o sin cpoint.
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
					CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nSe requiere un orden ascendente coherente entre areas (sin saltos) teniendo en cuenta las areas globales.").c_str());
				bool relative = area->Attribute("relative") && strcmp(area->Attribute("relative"),"true")==0;	// Rescatamos si las coordenadas son relativas.
				for (TiXmlElement* rectNode=area->FirstChildElement("rectangle");rectNode;rectNode=rectNode->NextSiblingElement()) { // Por cada rectangulo perteneciente al area.
					if (!(rectNode->Attribute("x1") && rectNode->Attribute("x2") && rectNode->Attribute("y1") && rectNode->Attribute("y2"))) 
						CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rectángulo de la imagen "+iinfo.name+" erróneos.").c_str());

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
						CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rectángulo de la imagen "+iinfo.name+" conflictivos.").c_str());

					rArea->push_back(new CRectangle(rc.x,rc.y,rc.w,rc.h));	// Finalmente creamos con ellas un rectangulo y lo añadimos al area actual.
				#ifdef LOG_SPRITESET_INFO
					printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x,rc.w,rc.y,rc.h);
					idebug++;
				#endif
				}
				area=area->NextSiblingElement();
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
						CLibrary::Error(("Estructura defectuosa del archivo: "+s_aux+"\nValores en un rectángulo global aplicado la imagen "+iinfo.name+" conflictivos.").c_str());

					rArea->push_back(new CRectangle(rc.x,rc.y,rc.w,rc.h)); // Finalmente creamos con ellas un rectangulo y lo añadimos al area actual.
				#ifdef LOG_SPRITESET_INFO
					printf("\tA%dR%d : x1=%d , x2=%d , y1=%d , y2=%d\n",numArea,idebug,rc.x,rc.w,rc.y,rc.h);
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
			img=img->NextSiblingElement();	// Si no es simple, buscamos la siguiente etiqueta imagen.
	}
	SDL_FreeSurface(chipset);
	chipset=NULL;

}
/*
void CSpriteset::operator delete(void* m) {
	delete ((CSpriteset*)m);
}*/
