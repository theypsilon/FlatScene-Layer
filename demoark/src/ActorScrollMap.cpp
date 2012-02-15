#include "ActorScrollMap.h"

#include "FSActor.h"
#include "FSLibrary.h"
#include "RutinaColision.h"
#include "FSUniverse.h"
#include "tests.h"

Uint32 CActorScrollMap::MSGID_Damage=FSMessageHandler::getNextMSGID();

CActorScrollMap::CActorScrollMap(const char* creature,FSMessageHandler * pmhParent) : 
FSActor(creature,pmhParent), dirx(0), diry(1), garbage(NULL),
actActual(NULL), actUltimo(NULL), actNeutro(NULL), actUltimoMovimiento(NULL), 
upfloor(false), downfloor(false), rutinaColision(CRutinaColision::collisionFactory()),
placeInMA(NULL) {
    
}
CActorScrollMap::~CActorScrollMap() {
    if (garbage) {
        delete garbage;
        garbage=NULL;
    }
    if (rutinaColision) {
        delete rutinaColision;
        rutinaColision=NULL;
    }
}
void CActorScrollMap::init(std::list<std::string>& activationIds)    { 
    // Nodos de Acciones:
    TiXmlDocument xmldoc("resources/actiontree.xml");

    if (!xmldoc.LoadFile()) FSLib.Error("actiontree.xml",TE_fileExists);    // Cargamos el archivo con la informaci�n referente a los nodos de acci�n.

    TiXmlHandle input = TiXmlHandle(xmldoc.FirstChild());

    std::list<TiXmlElement*> listActivateActions;

    const char* c[4]= {"Movenodes","Jumpnodes","Simpleattacknodes","Combonodes"};
    for (int i=0;i<4;i++) {
        TiXmlElement* actualAction = input.FirstChildElement(c[i]).FirstChildElement("action").ToElement();
        while (actualAction && actualAction->Attribute("type")) {
            TiXmlElement* actualNode = actualAction->FirstChildElement("node");
            while (actualNode && actualNode->Attribute("id")) {
                bool b=false;
                for (std::list<std::string>::iterator it=activationIds.begin();it!=activationIds.end() && !b;++it) {
                    b=strcmp((*it).c_str(),actualNode->Attribute("id"))==0;
                    if (b)
                        listActivateActions.push_back(actualNode);
                }
                if (b)  break;
                actualNode = actualNode->NextSiblingElement();
            }
            actualAction = actualAction->NextSiblingElement();
        }
    }

    CActionGraph actionTree(this);

    for (std::list<TiXmlElement*>::iterator it = listActivateActions.begin();it!=listActivateActions.end();++it) {
        if (!actionTree.add(*it))
            printf("Ese es %s\n",(*it)->Parent()->ToElement()->Attribute("type"));
    }

    if (!actionTree.mix())
        FSLib.Error("Error en la fase de mezcla del grafo de acciones");

    // actNeutro ser� la acci�nd e partida.
    actNeutro = actionTree.getNeutro();

    // los gr�ficos iniciales del Actor por tanto, ser�n los iniciales de actNeutro.
    flags = actNeutro->flags[actNeutro->seq];
    file = actNeutro->sptset[actNeutro->seq];
    graph = actNeutro->sequence[actNeutro->seq][actNeutro->paso>>3];

    garbage = actionTree.createGarbageCollector();

    addActionCandidate(actNeutro);

}

void CActorScrollMap::draw(FSPoint& offset) {

    FSPoint paux(m_Scrollxy.x,m_Scrollxy.y);
    paux.x -= offset.x;
    paux.y -= offset.y;

    getSprite()->put(paux,flags);

}

int CActorScrollMap::move()  { 

    if (actActual != NULL)
        return EXITO;
    else {
        FSLib.Error("ActorScrollMap no inicializado");
        return FRACASO;
    }

}
void CActorScrollMap::controlThis() {
    controlled=true;
}
void CActorScrollMap::noControlThis() {
    controlled=false;
}
bool CActorScrollMap::isControlled() {
    return controlled; 
}

void CActorScrollMap::setAction(CAction* n) {
    actActual=n;
    if (!actUltimo) {
        actActual->firstExecute();
        actUltimo=actActual;
    }
}

CAction* CActorScrollMap::getAction() {
    return actActual;
}

void CActorScrollMap::addActionCandidate(CAction* n) {
    eventChange=true;
    n->use();
    listActionCandidate.push_back(n);
}

void CActorScrollMap::removeActionCandidate(CAction* n) {
    eventChange=true;
    n->recycle();
    listActionCandidate.remove(n);
}

void CActorScrollMap::selectActionCandidate() {

    CAction* firstN;    // Preparamos la variable que contendr� la mejor acci�n candidata.

    eventChange=false;

    if (!listActionCandidate.empty()) { // Si hay alguna acci�n candidata.

        listActionCandidate.sort(CAction::orderReferenceNodes); // Ordenar lista de acciones candidatas seg�n prioridad.
        ActionCollection::iterator it=listActionCandidate.begin();

        firstN=*it; // Referenciamos la acci�n candidata de mayor prioridad.
        CAction* secondN=NULL;
        ++it;

        ActionCollection repN;  // Aqu� almacenaremos las acciones candidatas de igual prioridad si hace falta.

        while (it!=listActionCandidate.end()) { //Mientras hay m�s candidatas en la lista...
            secondN=firstN;
            firstN=*it;
            ++it;   // Apunta al siguiente elemento de la lista.
            if (secondN->getPriority()==firstN->getPriority()) { // Si son de la misma prioridad, la almacenamos en repN
                repN.push_back(secondN);
            } else {
                firstN=secondN; // Si no, nos quedamos con la de mayor prioridad y salimos.
                break;
            }
        }

        if (!repN.empty()) { // Si hay alguna acci�n repetida;
            firstN=firstN->mix(repN); // Las unimos.
            if (!firstN) {
                char c[1024];
                sprintf(c,"Mezcla de acciones ha dado mal resultado, responsable: %s con prioridad %d.",secondN->getId(),secondN->getPriority());
                FSLib.Error(c,TE_standard);
            }
        }

    } else 
        firstN=actNeutro;   // Si no hay acciones candidatas, asignamos por defecto la acci�n neutra.

    actActual=firstN; // Asignamos la mejor acci�n al personaje.
}

bool CActorScrollMap::collisionMap(int mov_x,int mov_y) {
    CMap* inUniverse = dynamic_cast<CMap*>(getUniverse());
    if (inUniverse!=NULL && getSprite()!=NULL && inUniverse->isLoaded()) {

        int invariantx=(m_Scrollxy.x+mov_x);
        int invarianty=(m_Scrollxy.y+mov_y);
        int invariantz= m_Scrollxy.z;
        int MAx = invariantx / inUniverse->getTileW();
        int MAy = invarianty / inUniverse->getTileH();

        if (MAx >= 0 && MAx < inUniverse->getW() && MAy >= 0 && MAy < inUniverse->getH()) {
            if ((mov_x + mov_y) == mov_x) {
                Uint32 colorSwitch=inUniverse->getPixel(invariantx,invarianty,invariantz);
                if (colorSwitch==0xFFF834F8) {
                    getParent()->SendMessage((CTestAGameInterface::MSGID_ChangeMap),(MSGPARM)this,(MSGPARM)mov_y);
                } else if (colorSwitch==0xFF202020) {
                    if (!upfloor) {
                        upfloor=true;
                        downfloor=false;
                        invariantz = ++m_Scrollxy.z;
                    }
                } else if (colorSwitch==0xFFF800F8) {
                    if (!downfloor) {
                        downfloor=true;
                        upfloor=false;
                        invariantz = --m_Scrollxy.z;
                    }
                } else {
                    downfloor=false;
                    upfloor=false;
                }

                if (placeInMA != inUniverse->MA[invariantz][MAx][MAy]) {
                    if (placeInMA)
                        placeInMA->remove(this);
                    placeInMA = inUniverse->MA[invariantz][MAx][MAy];
                    placeInMA->push_back(this);
                }

    #ifdef  MENSAJES_COLISIONCOLOR
                int colorDebug=(inUniverse->getPixel(invariantx,invarianty,m_Scrollxy.z));
                printf("%d          \r",colorDebug);
    #endif
            }
            return rutinaColision->run(invariantx,invarianty,invariantz,this);
        } else {
            if (placeInMA) {
                placeInMA->remove(this);
                placeInMA=NULL;
            }
            return true;
        }
    } else {
        return true;
    }
}


FSActor* CActorScrollMap::clone() {
    FSLib.Error("CActorScrollMap no implementa un metodo de clonaci�n por defecto.");
    return NULL;
}

bool CActorScrollMap::orderReferenceActors(CActorScrollMap* x, CActorScrollMap* y) {
    return ((x->m_Scrollxy.y)<(y->m_Scrollxy.y));
}

