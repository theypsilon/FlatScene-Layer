#include "IScrollLevel.h"
#include "tinyxml/tinyxml.h"
#include "IDebug.h"


IScrollLevel::IScrollLevel(std::string name) : Universe(name) {
#ifdef LOG_MAPAS
    printf("Creando mapa '%s'...\n",name.c_str());
#endif
}


IScrollLevel::~IScrollLevel(void)
{
    if (isLoaded())
        unload();
}

void IScrollLevel::load() {
#ifdef LOG_MAPAS
    printf("Cargando mapa '%s'...\n",name.c_str());
#endif

    numGates=0;

    if (name.at(name.size()-4)=='.')
        name = name.substr(0,name.size()-4);


    // Data from XML.


    TiXmlDocument xmldoc((name+".xml").c_str());

    if (!xmldoc.LoadFile()) {
        throw Exception("No se puede abrir el XML del mapa");
    }

    TiXmlHandle input(xmldoc.FirstChildElement("Map"));

    TiXmlElement* node = input.ToElement();

    if (!node) {
        throw Exception("Estructura XML defectuosa del mapa");
    }

    if (   !node->Attribute("width")        || !node->Attribute("height") 
        || !node->Attribute("layers")       || !node->Attribute("name")
        || !node->Attribute("tileWidth")    || !node->Attribute("tileHeight") ) 
    {
        throw Exception("Atributos XML defectuosos en el mapa");
    }

    axml ( node->QueryIntAttribute("width",(int*)&mapWidth) );
    axml ( node->QueryIntAttribute("height",(int*)&mapHeight) );
    axml ( node->QueryIntAttribute("layers",(int*)&numLayers) );

    axml ( node->QueryIntAttribute("tileWidth",(int*)&tileWidth) );
    axml ( node->QueryIntAttribute("tileHeight",(int*)&tileHeight) );

    datosTile=node->Attribute("name"); atxml(datosTile);

    /*
    if (node->QueryIntAttribute("precission-plus",&aux) == TIXML_SUCCESS)
    precissionPlus = (Uint8) aux;
    else
    precissionPlus=1;*/

    node = input.FirstChildElement("Music").FirstChildElement().ToElement();
    if (node && node->Attribute("name")) {
        datosSong = node->Attribute("name");
    }

    tileSet = durezaSet = -1;

    for (node = input.FirstChildElement("TileGraphs").FirstChildElement().ToElement();
         node && node->Attribute("name");   node = node->NextSiblingElement()) 
    {
        _tilesets.push_back(Spriteset(node->Attribute("name")));
    }

    for (node = input.FirstChildElement("TileCollisions").FirstChildElement().ToElement();
         node && node->Attribute("name");   node = node->NextSiblingElement()) 
    {
        _collisionsets.push_back(Spriteset(node->Attribute("name"),ONLY_SDL_SURFACE));
    }

    if (_collisionsets.empty() || _tilesets.empty()) {
        throw Exception("collisionsets or tilesets empty");
    }

    LayerType.clear();
    LayerFloor.clear();

    for (node = input.FirstChildElement("LayerList").FirstChildElement().ToElement();
         node && node->Attribute("type") && node->Attribute("floor");
         node = node->NextSiblingElement()) 
    {
        const char* auxcad = node->Attribute("type");

        int layertype;

        if (strcmp(auxcad,"background")==0) {
            layertype = 0;
        } else if (strcmp(auxcad,"lower")==0) {
            layertype = 1;
        } else if (strcmp(auxcad,"upper")==0) {
            layertype = 2;
        } else {
            throw Exception("LayerType invalidado cargando el mapa");
            return;
        }

        LayerType.push_back(layertype);

        int floorattr;

        axml( node->QueryIntAttribute("floor",&floorattr) );
        LayerFloor.push_back( floorattr );
    }

    if (LayerType.size() != numLayers) {
        throw Exception("Definicion de capas defectuosa en el mapa");
    }

    for (node = input.FirstChildElement("GateList").FirstChildElement().ToElement();
        node && node->Attribute("target")   && node->Attribute("x1")        && node->Attribute("x2") 
             && node->Attribute("y1")       && node->Attribute("y2")        && node->Attribute("z") 
             && node->Attribute("target-x") && node->Attribute("target-y")  && node->Attribute("target-z");
        node = node->NextSiblingElement()) 
    {
        numGates++;
    }

    Gates.clear();

    for (node = input.FirstChildElement("GateList").FirstChildElement("Gate").ToElement();
         node;  node = node->NextSiblingElement("Gate")) 
    {
        gate g;

        g.destino = node->Attribute("target"); atxml( g.destino );
        axml ( node->QueryIntAttribute("x1",(int*) & (g.regionx1)) );
        axml ( node->QueryIntAttribute("x2",(int*) & (g.regionx2)) );
        axml ( node->QueryIntAttribute("y1",(int*) & (g.regiony1)) );
        axml ( node->QueryIntAttribute("y2",(int*) & (g.regiony2)) );
        axml ( node->QueryIntAttribute("z",(int*) & (g.regionz)) );
        axml ( node->QueryIntAttribute("target-x",(int*) & (g.destino_scroll_x)) );
        axml ( node->QueryIntAttribute("target-y",(int*) & (g.destino_scroll_y)) );
        axml ( node->QueryIntAttribute("target-z",(int*) & (g.destino_scroll_z)) );

        Gates.push_back(g);
    }

    if (Gates.size()!=numGates) {
        throw Exception("Definicion de gates defectuosa en el mapa");
    }

    xmldoc.Clear();


    // Data from binay file.


    FILE* f_map;
#ifdef _MSC_VER
    if (fopen_s(&f_map,(name+".dat").c_str(),"rb")==-1 || f_map==nullptr) {
#else
    if ((f_map=fopen((name+".dat").c_str(),"rb"))==NULL) {
#endif
        throw Exception(std::string("El mapa '\n")+name+"' no se encuentra.");
    }

    char buffer[5]="";

    fread(&buffer,sizeof(char),4,f_map);

    if (strcmp(buffer,"JMBM")!=0) {
        throw Exception("Firma del mapa invalida");
    }

    {
        TileBG tileData;

        for (unsigned int i=0;i<numLayers;i++) {

            ITileAndDur tillayer;
            tillayer.tiles.resize(mapHeight);
            tillayer.tile = new TileBG*[mapHeight];
            tillayer.dur = false;

            for (unsigned int j=0;j<mapHeight;j++) {
                tillayer.tiles[j].reserve(mapWidth);
                tillayer.tile[j] = new TileBG[mapWidth];
                for  (unsigned int k=0;k<mapWidth;k++) {
                    fread(&tileData, sizeof(TileBG),1,f_map);
                    tillayer.tiles[j].push_back( Tile(
                        *_tilesets[tileData.fileGraph].get(tileData.graph),
                        (unsigned short) tileData.flags,
                        tileData.dur > 0 ? _collisionsets[tileData.fileDur].get(tileData.dur - 1) : nullptr,
                        (unsigned short) tileData.vacio
                    ));
                    tillayer.tile[j][k] = tileData;
                }
            }

            layerlvl.push_back(tillayer);
        }
    }

    fread(&buffer,sizeof(char),4,f_map);

    if (strcmp(buffer,"JMBM")!=0) {
        throw Exception("Firma del mapa invalida");
    }

    fclose(f_map);


    // Generating Map structure

    for (unsigned int i= 0; i< numLayers; i++) {

        layerlvl[i].dur = false;

        for (unsigned int layer = LayerFloor[i]; (i < numLayers) && (LayerFloor[i] == layer); i++) {
            if (layerlvl[i].dur)
                continue;

            if (LayerType[i] == 1) {
                layerlvl[i].dur = true;

                _linktodur.push_back( &layerlvl[i].tiles );
            }
        }
    }

    MA.clear();

    for (unsigned int z=0;z<=LayerFloor[numLayers-1];z++) {
        MA.push_back( new IScrollObjectCollection**[mapWidth] );
        for (unsigned int x=0;x<mapWidth;x++) {
            MA[z][x] = new IScrollObjectCollection*[mapHeight];
            for (unsigned int y=0;y<mapHeight;y++) {
                MA[z][x][y]=new IScrollObjectCollection;
            }
        }
    }

    for (auto it = actor.begin();it!=actor.end();++it) {
        IScrollObject* actscroll = dynamic_cast<IScrollObject*>(*it);

        if (actscroll) {
            unsigned int MAz = actscroll->place.z,
                MAx = actscroll->place.x / getTileW(),
                MAy = actscroll->place.y / getTileH();
            if (MAz >=0 && MAx >= 0 && MAy >= 0 && MAz <= LayerFloor[numLayers-1] && MAx < getW() && MAy < getH()) {
                actscroll->placeInMA = MA[MAz][MAx][MAy];
                actscroll->placeInMA->push_back(actscroll);
            } else {
                actscroll->placeInMA = nullptr;
            }
        }
    }


    //	The map is ready.


    loaded=true;
}


void IScrollLevel::unload() {
#ifdef LOG_MAPAS
    printf("Liberando mapa '%s'...\n",name.c_str());
#endif

    for (unsigned int z=0;z < MA.size();z++) {
        for (unsigned int x=0;x<mapWidth;x++) {
            for (unsigned int y=0;y<mapHeight;y++) {
                delete MA[z][x][y];
            }
            delete MA[z][x];
        }
        delete MA[z];
    }

    MA.clear();

    for (auto it = layerlvl.begin(); it != layerlvl.end() ; ++it) {
        TileBG** tile = (*it).tile;
        for (unsigned int j=0;j<mapHeight;j++) {
            delete (tile[j]);
        }
        delete (tile);
    }

    layerlvl.clear();
    _linktodur.clear();
    LayerType.clear();
    LayerFloor.clear();
    Gates.clear();

    for (int i=tileSet,j=lastTileset;i<=j;i++)
        Img.remove(i);

    for (int i=durezaSet,j=lastDurezaset;i<=j;i++)
        Img.remove(i);

    loaded=false;
}

int IScrollLevel::incActor(Actor* act) {
    IScrollObject* actscroll = dynamic_cast<IScrollObject*>(act);

    if (actscroll && act->getUniverse()==NULL) {
        act->setUniverse(this);
        actor.push_back(act);
        if (isLoaded()) {
            unsigned int MAz = actscroll->place.x,
                MAx = actscroll->place.x / getTileW(),
                MAy = actscroll->place.y / getTileH();
            if (MAz >=0 && MAx >= 0 && MAy >= 0 && MAz <= LayerFloor[numLayers-1] && MAx < getW() && MAy < getH()) {
                actscroll->placeInMA = MA[MAz][MAx][MAy];
                actscroll->placeInMA->push_back(actscroll);
            } else {
                actscroll->placeInMA = NULL;
            }
        }
        return EXITO;
    } else {
        throw Exception((std::string("Se ha a�adido un actor al mapa ")+getName()+std::string(" perteneciendo actualmente a ")+act->getUniverse()->getName()).c_str());
        return FRACASO;
    }
}

int IScrollLevel::decActor(Actor* act) {
    IScrollObject* actscroll = dynamic_cast<IScrollObject*>(act);

    for (auto i=actor.begin();i!=actor.end();++i) {
        if (act==*i) {
            actor.erase(i);
            if (isLoaded()) {
                actscroll->placeInMA->remove(actscroll);
                actscroll->setUniverse(NULL);
                actscroll->placeInMA = NULL;
            }
            break;
        }
    }
    /*if (actor.size()==0) {
    if (getParent()!=NULL) {
    CTestAGameInterface* father=(CTestAGameInterface*)getParent();
    father->SendMessage(CTestAGameInterface::MSGID_DeleteMap,(MSGPARM)this);
    unload();
    }
    }*/

    return EXITO;
}

Uint32 IScrollLevel::getPixel(int x, int y,int z) {
    unsigned int j=x/tileWidth,
        i=y/tileHeight;
    if (j>=0 && i>=0 && j<mapWidth && i<mapHeight) {
        const Tile& durtile = (*_linktodur[z])[i][j];

        if (durtile.mcollision) {

            x=x%(durtile.mcollision->getWidth());
            y=y%(durtile.mcollision->getHeight());

            /*x=x%(durtile.mcollision->getWidth()*2);
            x/=2;
            y=y%(durtile.mcollision->getHeight()*2);
            y/=2;*/

            if (durtile.mflags & 0x01) {
                x = durtile.mcollision->getWidth() - x -1;
            } 

            if (durtile.mflags > 1) {
                y = durtile.mcollision->getHeight() - y -1;
            }

            return durtile.mcollision->getPixel(x,y);

        }
        return 0;
    }
    return 2575;
}
