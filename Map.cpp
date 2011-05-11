#include "Map.h"
#include "TestAGameInterface.h"
#include "FSLibrary.h"

CMap::CMap(string name) : CUniverse(name) {
#ifdef LOG_MAPAS
	printf("Creando mapa '%s'...\n",name.c_str());
#endif
}

CMap::~CMap() {
	if (isLoaded())
		unload();
#ifdef LOG_MAPAS
	printf("Destruyendo mapa '%s'...\n",name.c_str());
#endif
}

string& CMap::getName() {
	return name;
}

void CMap::load() {
#ifdef LOG_MAPAS
	printf("Cargando mapa '%s'...\n",name.c_str());
#endif

	int aux=0;
	numGates=0;


	// Data from XML.


	TiXmlDocument xmldoc(resource((name+".xml").c_str()).c_str());

	if (!xmldoc.LoadFile()) {
		CLibrary::Error("No se puede abrir el XML del mapa");
		return;
	}

	TiXmlHandle input(xmldoc.FirstChildElement("Map"));

	TiXmlElement* node = input.ToElement();

	if (!node) {
		CLibrary::Error("Estructura XML defectuosa del mapa");
		return;
	}

	if (!node->Attribute("width") || !node->Attribute("height") || !node->Attribute("layers") || !node->Attribute("tileWidth") || !node->Attribute("tileHeight") || !node->Attribute("name")) {
		CLibrary::Error("Atributos XML defectuosos en el mapa");
		return;
	}

	node->QueryIntAttribute("width",(int*)&mapWidth);
	node->QueryIntAttribute("height",(int*)&mapHeight);
	node->QueryIntAttribute("layers",(int*)&numLayers);

	node->QueryIntAttribute("tileWidth",(int*)&tileWidth);
	node->QueryIntAttribute("tileHeight",(int*)&tileHeight);

	datosTile = node->Attribute("name");

	if (node->QueryIntAttribute("precission-plus",&aux) == TIXML_SUCCESS)
		precissionPlus = (Uint8) aux;
	else
		precissionPlus=1;

	aux= 0;

	node = input.FirstChildElement("Music").FirstChildElement().ToElement();
	if (node && node->Attribute("name"))
		datosSong = node->Attribute("name");

	tileSet = durezaSet = -1;

	for (node = input.FirstChildElement("TileGraphs").FirstChildElement().ToElement();node && node->Attribute("name");node = node->NextSiblingElement()) {
		if (tileSet == -1) 
			tileSet = lastTileset = CImg.add(node->Attribute("name"));
		else
			lastTileset = CImg.add(node->Attribute("name"));
	}

	for (node = input.FirstChildElement("TileCollisions").FirstChildElement().ToElement();node && node->Attribute("name");node = node->NextSiblingElement()) {
		if (durezaSet == -1)
			durezaSet = lastDurezaset = CImg.add(node->Attribute("name"),ONLY_SDL_SURFACE);
		else
			lastDurezaset = CImg.add(node->Attribute("name"));
	}

	if (durezaSet == -1 || tileSet == -1) {
		CLibrary::Error("TileGraphs y/o TileCollisions defectuosos en el mapa");
		return;
	}

	LayerType= alloc(Uint32,numLayers);
	LayerFloor= alloc(Uint32,numLayers);

	for (node = input.FirstChildElement("LayerList").FirstChildElement().ToElement();
			node && node->Attribute("type") && node->Attribute("floor");
			node = node->NextSiblingElement()) {

		const char* auxcad = node->Attribute("type");

		if (strcmp(auxcad,"background")==0) {
			LayerType[aux]=0;
		} else if (strcmp(auxcad,"lower")==0) {
			LayerType[aux]=1;
		} else if (strcmp(auxcad,"upper")==0) {
			LayerType[aux]=2;
		} else {
			CLibrary::Error("LayerType invalidado cargando el mapa");
			return;
		}

		node->QueryIntAttribute("floor",(int*)& LayerFloor[aux]);

		aux++;
	}

	if (aux != numLayers) {
		CLibrary::Error("Definicion de capas defectuosa en el mapa");
		return;
	}

	for (node = input.FirstChildElement("GateList").FirstChildElement().ToElement();
			node && node->Attribute("target") && node->Attribute("x1") && node->Attribute("x2") && node->Attribute("y1") && node->Attribute("y2") && node->Attribute("z") && node->Attribute("target-x") && node->Attribute("target-y") && node->Attribute("target-z");
			node = node->NextSiblingElement()) {

		numGates++;
	}

	Gates = new gate[numGates];
	
	aux = 0;

	for (node = input.FirstChildElement("GateList").FirstChildElement().ToElement();node;node = node->NextSiblingElement()) {
		gate* g= &Gates[aux];
		aux++;

		g->destino = node->Attribute("target");
		node->QueryIntAttribute("x1",(int*) & (g->regionx1));
		node->QueryIntAttribute("x2",(int*) & (g->regionx2));
		node->QueryIntAttribute("y1",(int*) & (g->regiony1));
		node->QueryIntAttribute("y2",(int*) & (g->regiony2));
		node->QueryIntAttribute("z",(int*) & (g->regionz));
		node->QueryIntAttribute("target-x",(int*) & (g->destino_scroll_x));
		node->QueryIntAttribute("target-y",(int*) & (g->destino_scroll_y));
		node->QueryIntAttribute("target-z",(int*) & (g->destino_scroll_z));
	}

	if (aux!=numGates) {
		CLibrary::Error("Definicion de gates defectuosa en el mapa");
		return;
	}

	xmldoc.Clear();


	// Data from binay file.

	
	FILE* f_map;
	if ((f_map=fopen(resource((name+".dat").c_str()).c_str(),"rb"))==NULL) {
		 CLibrary::Error(string("El mapa '\n")+name+"' no se encuentra.");
		 return;
	}

	char buffer[5]="";

	fread(&buffer,sizeof(char),4,f_map);

	if (strcmp(buffer,"JMBM")!=0) {
		CLibrary::Error("Firma del mapa invalida");
		return;
	}

	tile = alloc(TileBG**,numLayers);
	
	for (int i=0;i<numLayers;i++) {
		tile[i] = alloc(TileBG*,mapHeight);
		for (int j=0;j<mapHeight;j++) {
			tile[i][j]= alloc(TileBG,mapWidth);
			for (int k=0;k<mapWidth;k++) {
				fread(&tile[i][j][k],sizeof(TileBG),1,f_map);
			}
		}
	}

	fread(&buffer,sizeof(char),4,f_map);

	if (strcmp(buffer,"JMBM")!=0) {
		CLibrary::Error("Firma del mapa invalida");
		return;
	}

	fclose(f_map);


	// Generating Map structure


	int i=0;
	dur= alloc(TileBG**,LayerFloor[numLayers-1]+1);
	for (int c=0;c<=LayerFloor[numLayers-1];c++) {
		bool primera=false;
		// TODO : posible bug por la expresión ((primera==false)||(LayerFloor[i-1]==LayerFloor[i])
		for (;(i<numLayers)&&((primera==false)||(LayerFloor[i-1]==LayerFloor[i]));i++) {
			if (!primera && LayerType[i]==1) {
				primera=true;
				dur[c]= alloc(TileBG*,mapHeight);
				
				for (int j=0;j<mapHeight;j++) {
					dur[c][j]= alloc(TileBG,mapWidth);
					for (int k=0;k<mapWidth;k++) {
						dur[c][j][k]=tile[i][j][k];
					}
				}
			}
		}

	}

	MA = alloc(ActorScrollCollection***,1+LayerFloor[numLayers-1]);
	for (int z=0;z<=LayerFloor[numLayers-1];z++) {
		MA[z] = alloc(ActorScrollCollection**,mapWidth);
		for (int x=0;x<mapWidth;x++) {
			MA[z][x]= alloc(ActorScrollCollection*,mapHeight);
			for (int y=0;y<mapHeight;y++) {
				MA[z][x][y]=new ActorScrollCollection;
			}
		}
	}

	for (ActorCollection::iterator it = actor.begin();it!=actor.end();++it) {
		CActorScrollMap* actscroll = dynamic_cast<CActorScrollMap*>(*it);

		if (actscroll) {
			int MAz = actscroll->m_Scrollxy.Z() ;
			int MAx = actscroll->m_Scrollxy.X() / getTileW();
			int MAy = actscroll->m_Scrollxy.Y() / getTileH();
			if (MAz >=0 && MAx >= 0 && MAy >= 0 && MAz <= LayerFloor[numLayers-1] && MAx < getW() && MAy < getH()) {
				actscroll->placeInMA = MA[MAz][MAx][MAy];
				actscroll->placeInMA->push_back(actscroll);
			} else {
				actscroll->placeInMA = NULL;
			}
		}
	}


	//	The map is ready.


	loaded=true;
}

void CMap::unload() {
#ifdef LOG_MAPAS
	printf("Liberando mapa '%s'...\n",name.c_str());
#endif
	
	for (int z=0;z<=LayerFloor[numLayers-1];z++) {
		for (int x=0;x<mapWidth;x++) {
			for (int y=0;y<mapHeight;y++) {
				delete MA[z][x][y];
			}
			freemem(MA[z][x]);
		}
		freemem(MA[z]);
	}
	freemem(MA);

	for (int i=0;i<numLayers;i++) {	
		for (int j=0;j<mapHeight;j++) {
			freemem(tile[i][j]);
		}
		freemem(tile[i]);
	}
	freemem(tile);

	for (int i=0;i<=LayerFloor[numLayers-1];i++) {
		for (int j=0;j<mapHeight;j++) {
			freemem(dur[i][j]);
		}
		freemem(dur[i]);
	}
	freemem(dur);

	freemem(LayerType);
	freemem(LayerFloor);

	delete[] Gates;

	for (int i=tileSet,j=lastTileset;i<=j;i++)
		CImg.remove(i);

	for (int i=durezaSet,j=lastDurezaset;i<=j;i++)
		CImg.remove(i);

	loaded=false;
}

int CMap::incActor(CActor* act) {
	CActorScrollMap* actscroll = dynamic_cast<CActorScrollMap*>(act);

	if (actscroll && act->getUniverse()==NULL) {
		act->setUniverse(this);
		actor.push_back(act);
		if (isLoaded()) {
			int MAz = actscroll->m_Scrollxy.Z() ;
			int MAx = actscroll->m_Scrollxy.X() / getTileW();
			int MAy = actscroll->m_Scrollxy.Y() / getTileH();
			if (MAz >=0 && MAx >= 0 && MAy >= 0 && MAz <= LayerFloor[numLayers-1] && MAx < getW() && MAy < getH()) {
				actscroll->placeInMA = MA[MAz][MAx][MAy];
				actscroll->placeInMA->push_back(actscroll);
			} else {
				actscroll->placeInMA = NULL;
			}
		}
		return EXITO;
	} else {
		CLibrary::Error((string("Se ha añadido un actor al mapa ")+getName()+string(" perteneciendo actualmente a ")+act->getUniverse()->getName()).c_str());
		return FRACASO;
	}
}

int CMap::decActor(CActor* act) {
	CActorScrollMap* actscroll = dynamic_cast<CActorScrollMap*>(act);

	for (ActorCollection::iterator i=actor.begin();i!=actor.end();++i) {
		if (act==*i) {
			actor.erase(i);
			if (isLoaded()) {
				actscroll->placeInMA->remove(actscroll);
				act->setUniverse(NULL);
			}
			break;
		}
	}
	if (actor.size()==0) {
		if (getParent()!=NULL) {
			CTestAGameInterface* father=(CTestAGameInterface*)getParent();
			father->SendMessage(CTestAGameInterface::MSGID_DeleteMap,(MSGPARM)this);
			unload();
		}
	}

	return EXITO;
}

inline Uint32 CMap::getW() {
	return mapWidth;
}

inline Uint32 CMap::getH() {
	return mapHeight;
}


Uint32 CMap::getLayer() {
	return numLayers;
}

inline Uint32 CMap::getTileW() {
	return tileWidth;
}

inline Uint32 CMap::getTileH() {
	return tileHeight;
}

TileBG*** CMap::getTiles() {
	return tile;
}

TileBG*** CMap::getDurezas() {
	return dur;
}

Uint32 CMap::getTileset() {
	return tileSet;
}

Uint32 CMap::getDurezaset() {
	return durezaSet;
}

Uint32 CMap::getLastTileset() {
	return lastTileset;
}

Uint32 CMap::getLastDurezaset() {
	return lastDurezaset;
}

Uint32* CMap::getLayerFloor() {
	return LayerFloor;
}

Uint32* CMap::getLayerType() {
	return LayerType;
}

Uint8 CMap::getPrecissionPlus() {
	return precissionPlus;
}

Uint8 CMap::setPrecissionPlus(Uint8 n) {
	Uint8 aux = precissionPlus;
	precissionPlus=n;
	return aux;
}


#ifdef MENSAJES_MSGIDS
int CMap::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
	printf("Mapa %s :: ",getName().c_str());
	return CMessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
}
#endif

void CMap::rescataChar(string& cad,int size, int n, FILE *f) {
	char c;
	cad.clear();
	for (int i=0;i<size;i++) {
		fread(&c,sizeof(char),n,f);
		if (c!='\\')
			cad+=c;
	}
}

Uint32 CMap::getPixel(int x, int y,int z) {
	int j=x/getTileW();
	int i=y/getTileH();
	if (j>=0 && i>=0 && j<mapWidth && i<mapHeight) {
		int tile_pisado=dur[z][i][j].dur;
		
		if (tile_pisado>0) {
			tile_pisado--;
			CSprite* canv=CImg.get(durezaSet+dur[z][i][j].fileDur)->get(tile_pisado);

			int flags = dur[z][i][j].flags;

			x=x%(canv->getWidth());
			y=y%(canv->getHeight());

			/*x=x%(canv->getWidth()*2);
			x/=2;
			y=y%(canv->getHeight()*2);
			y/=2;*/

			if (flags & 0x01) {
				x=canv->getWidth() - x -1;
			} 
				
			if (flags > 1) {
				y=canv->getHeight() - y -1;
			}			

			return (canv->getPixel(x,y));
			
		}
		return 0;
	}
	return 2575;
}
