#include "IScrollLevel.h"
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

    using namespace U::XML::Tiny;

    _mapWidth = intFromAttr(*node,"width");
    _mapHeight = intFromAttr(*node,"height");
    _numLayers = intFromAttr(*node,"layers");

    _tileWidth = intFromAttr(*node,"tileWidth");
    _tileHeight = intFromAttr(*node,"tileHeight");

    _datosTile = valFromAttr<std::string>(*node,"name");

    /*
    if (node->QueryIntAttribute("precission-plus",&aux) == TIXML_SUCCESS)
    precissionPlus = (Uint8) aux;
    else
    precissionPlus=1;*/

    node = input.FirstChildElement("Music").FirstChildElement().ToElement();

    _datosSong = node? valFromAttr<std::string>(*node,"name") : "";

    for (node = input.FirstChildElement("TileGraphs").FirstChildElement().ToElement();
         node && node->Attribute("name");   node = node->NextSiblingElement()) 
    {
        _tilesets.push_back(Spriteset(valFromAttr<std::string>(*node,"name")));
    }

    for (node = input.FirstChildElement("TileCollisions").FirstChildElement().ToElement();
         node && node->Attribute("name");   node = node->NextSiblingElement()) 
    {
        _collisionsets.push_back(Spriteset(valFromAttr<std::string>(*node,"name")/*,ONLY_CPU*/));
    }

    if (_collisionsets.empty() || _tilesets.empty()) {
        throw Exception("collisionsets or tilesets empty");
    }

    _layerType.clear();
    _layerFloor.clear();

    for (node = input.FirstChildElement("LayerList").FirstChildElement().ToElement();
         node && checkAttr(*node,"type") && checkAttr(*node,"floor");
         node = node->NextSiblingElement()) 
    {
        const std::string auxcad = valFromAttr<std::string>(*node,"type");

        int layertype;

        if (auxcad == "background") {
            layertype = 0;
        } else if (auxcad == "lower") {
            layertype = 1;
        } else if (auxcad == "upper") {
            layertype = 2;
        } else {
            throw Exception("LayerType invalidado cargando el mapa");
        }

        _layerType.push_back(layertype);

        int floorattr = intFromAttr(*node,"floor");
        _layerFloor.push_back( floorattr );
    }

    if (_layerType.size() != getL()) {
        throw Exception("Definicion de capas defectuosa en el mapa");
    }

    _numGates=0;
    for (node = input.FirstChildElement("GateList").FirstChildElement().ToElement();
        node && checkAttr(*node,"target")   && checkAttr(*node,"x1")        && checkAttr(*node,"x2") 
             && checkAttr(*node,"y1")       && checkAttr(*node,"y2")        && checkAttr(*node,"z") 
             && checkAttr(*node,"target-x") && checkAttr(*node,"target-y")  && checkAttr(*node,"target-z");
        node = node->NextSiblingElement()) 
    {
        _numGates++;
    }

    _gates.clear();

    for (node = input.FirstChildElement("GateList").FirstChildElement("Gate").ToElement();
         node;  node = node->NextSiblingElement("Gate")) 
    {
        Gate g;

        g.destino = valFromAttr<std::string>(*node,"target");

        g.regionx1 = intFromAttr(*node,"x1");
        g.regionx2 = intFromAttr(*node,"x2");
        g.regiony1 = intFromAttr(*node,"y1");
        g.regiony2 = intFromAttr(*node,"y2");
        g.regionz = intFromAttr(*node,"z");
        g.destino_scroll_x = intFromAttr(*node,"target-x");
        g.destino_scroll_y = intFromAttr(*node,"target-y");
        g.destino_scroll_z = intFromAttr(*node,"target-z");

        _gates.push_back(g);
    }

    if (_gates.size()!=_numGates) {
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
        _layerlvl.resize(getL());

        for (auto& tillayer : _layerlvl) {

            tillayer.tiles.resize(getH());
            tillayer.dur = false;

            for (auto& tilsublayer : tillayer.tiles) {
                tilsublayer.reserve(getW());
                for  (unsigned int k=0; k < getW(); k++) {
                    fread(&tileData, sizeof(TileBG),1,f_map);
                    tilsublayer.push_back( Tile(
                        tileData.graph > 0 ? &_tilesets     [tileData.fileGraph]->at(tileData.graph -1) : nullptr,
                        tileData.dur   > 0 ? &_collisionsets[tileData.fileDur  ]->at(tileData.dur - 1 ) : nullptr,
                        (unsigned short) tileData.flags
                    ));
                }
            }
        }
    }

    fread(&buffer,sizeof(char),4,f_map);

    if (strcmp(buffer,"JMBM")!=0) {
        throw Exception("Firma del mapa invalida");
    }

    fclose(f_map);


    // Generating Map structure

    for (unsigned int i= 0; i< getL(); i++) {

        _layerlvl[i].dur = false;

        for (unsigned int layer = _layerFloor[i]; (i < getL()) && (_layerFloor[i] == layer); i++) {
            if (_layerlvl[i].dur)
                continue;

            if (_layerType[i] == 1) {
                _layerlvl[i].dur = true;

                _linktodur.push_back( &_layerlvl[i].tiles );
            }
        }
    }

    _MA.clear();
    _MA.resize(_layerFloor[getL()-1]); //FIXME ensure its this right, or it would be _MA.resize(LayerFloor[numLayers-1]+1);
    for (unsigned int z = 0; z < _MA.size(); z++) {
        _MA[z].resize(getW());
        for (unsigned int x = 0; x < getW(); x++) {
            _MA[z][x].resize(getH());
        }            
    }

    for (auto& a : actor) {
        IScrollObject* actscroll = dynamic_cast<IScrollObject*>(a);
        if (actscroll) {
            unsigned int MAz = actscroll->place.z,
                         MAx = actscroll->place.x / getTileW(),
                         MAy = actscroll->place.y / getTileH();
            if (MAz < _MA.size() && MAx < getW() && MAy < getH()) {
                actscroll->_placeInMA = &_MA[MAz][MAx][MAy];
                _MA[MAz][MAx][MAy].push_back(actscroll);
            } else {
                actscroll->_placeInMA = nullptr;
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

    _layerlvl.clear();
    _linktodur.clear();
    _layerType.clear();
    _layerFloor.clear();
    _gates.clear();

    loaded=false;
}

int IScrollLevel::incActor(Cinema::Actor* act) {
    IScrollObject* actscroll = dynamic_cast<IScrollObject*>(act);

    if (actscroll && act->getUniverse()==nullptr) {
        act->setUniverse(this);
        actor.push_back(act);
        if (isLoaded()) {
            unsigned int MAz = actscroll->place.z,
                         MAx = actscroll->place.x / getTileW(),
                         MAy = actscroll->place.y / getTileH();
            if (MAz < _MA.size() && MAx < getW() && MAy < getH()) {
                actscroll->_placeInMA = &_MA[MAz][MAx][MAy];
                _MA[MAz][MAx][MAy].push_back(actscroll);
            } else {
                actscroll->_placeInMA = nullptr;
            }
        }
        return EXIT_SUCCESS;
    } else {
        throw Exception((std::string("Se ha a�adido un actor al mapa ")+getName()+std::string(" perteneciendo actualmente a ")+act->getUniverse()->getName()).c_str());
        return EXIT_FAILURE;
    }
}

int IScrollLevel::decActor(Cinema::Actor* act) {
    IScrollObject* actscroll = dynamic_cast<IScrollObject*>(act);

    for (auto i=actor.begin();i!=actor.end();++i) {
        if (act==*i) {
            actor.erase(i);
            if (isLoaded()) {
                actscroll->_placeInMA->remove(actscroll);
                actscroll->setUniverse(nullptr);
                actscroll->_placeInMA = nullptr;
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

    return EXIT_SUCCESS;
}

unsigned int IScrollLevel::getPixel(int x, int y,int z) {
    unsigned int j=x / getTileW(),
                 i=y / getTileH();
    if (j >= 0 && i >= 0 && j < getW() && i < getH()) {
        const Tile& durtile = (*_linktodur[z])[i][j];

        if (durtile.getCollision()) {

            x=x%(durtile.getCollision()->getWidth());
            y=y%(durtile.getCollision()->getHeight());

            /*x=x%(durtile.mcollision->getWidth()*2);
            x/=2;
            y=y%(durtile.mcollision->getHeight()*2);
            y/=2;*/

            if (durtile.getFlags() & 0x01) {
                x = durtile.getCollision()->getWidth() - x -1;
            } 

            if (durtile.getFlags() > 1) {
                y = durtile.getCollision()->getHeight() - y -1;
            }

            return durtile.getCollision()->getPixel(x,y);

        }
        return 0;
    }
    return 2575;
}
