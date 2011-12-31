#include "OrderGraph.h"
#include "FSLibrary.h"

CEnlace::CEnlace(string referencia,string tecla,string evento) : referencia(referencia), tecla(tecla), evento(evento) {}
CEnlace::~CEnlace() {}

COrder::COrder(CAction* n, string identidad) : nodo(n), identidad(identidad), prioridad(-1) {}
COrder::~COrder() {}

COrderGraph::COrderGraph() {}
COrderGraph::~COrderGraph() {}

bool COrderGraph::mix() {
	return false;
}

CActionGraph::CActionGraph(CActorScrollMap* player) : actNeutro(NULL), player(player) {
	numTipos=12;
	tipos =alloc(const char*,numTipos);
	tipos[0] = "quiet"; tipos[1]="walk"; tipos[2]="run"; tipos[3]="jump"; tipos[4]="long jump"; tipos[5]="normal basic attack"; tipos[6]="normal super attack"; tipos[7]="onslaught super attack"; tipos[8]="push basic attack"; tipos[9]="air basic attack"; tipos[10]="air super attack"; tipos[11]="air-land super attack" ;
}

CActionGraph::~CActionGraph() {
	freemem(tipos);
	for (list<COrder*>::iterator it=nodos.begin();it!=nodos.end();++it) {
//		delete (*it)->nodo;
		delete (*it);
	}

}

COrder* CActionGraph::createNode(const char *c, TiXmlElement *newNode,const char* s) {
	if (!newNode->Attribute("id")) FSLibrary::Error("Acción sin identificador imposible de crear.");
	COrder* newInstance = NULL;
	string name = (s==NULL)?string(c) : string(c)+s;
	if (strcmp(c,"quiet")==0) {
		if (validateMoveAttributes(newNode) && !actNeutro)	{
			newInstance = new COrder( CActionMove::ActionMoveFactory(player,newNode) , name);
			actNeutro=newInstance;
		}
	} else if (strcmp(c,"walk")==0) {
		if (validateMoveAttributes(newNode))
			newInstance = new COrder( CActionMove::ActionMoveFactory(player,newNode) , name);
	} else if (strcmp(c,"run")==0) {
		if (validateMoveAttributes(newNode))
			newInstance = new COrder( CActionMove::ActionMoveFactory(player,newNode), name ); // (CAction*) new CActionMove(player,atoi(newNode->Attribute("x")),atoi(newNode->Attribute("y")),atoi(newNode->Attribute("ppf")),"run") 
	} else if (strcmp(c,"jump")==0) {
		newInstance = new COrder( CActionFight::ActionFightFactory(player,newNode) , name );
	} else if (strcmp(c,"long jump")==0) {
		newInstance = new COrder( CActionFight::ActionFightFactory(player,newNode)  , name);
	} else if (strcmp(c,"normal basic attack")==0) {
		newInstance = new COrder( CActionFight::ActionFightFactory(player,newNode) , name );
	} else if (strcmp(c,"normal super attack")==0) {
		newInstance = new COrder( CActionFight::ActionFightFactory(player,newNode) , name );
	} else if (strcmp(c,"onslaught super attack")==0) {
		newInstance = new COrder( CActionFight::ActionFightFactory(player,newNode) , name );
	} else if (strcmp(c,"push basic attack")==0) {
		newInstance = new COrder( CActionFight::ActionFightFactory(player,newNode) , name );
	} else if (strcmp(c,"air basic attack")==0) {
		newInstance = new COrder( CActionFight::ActionFightFactory(player,newNode) , name );
	} else if (strcmp(c,"air super attack")==0) {
		newInstance = new COrder( CActionFight::ActionFightFactory(player,newNode) , name );
	} else if (strcmp(c,"air-land super attack")==0) {
		newInstance = new COrder( CActionFight::ActionFightFactory(player,newNode)  , name);
	} else if (strcmp(c,"restringed")==0) {
		FSLibrary::Error("Se ha intentado agregar un tipo de acción restringido.");
	}
	return newInstance;
}

bool CActionGraph::add(TiXmlElement *newNode) {
	TiXmlElement* parentNode=newNode->Parent()->ToElement();
	if ( parentNode && parentNode->Attribute("type") && strcmp(parentNode->Value(),"action")==0 ) {
		if (strcmp(newNode->Value(),"node")!=0) {
			return false;
		}

		int tipoActual=-1;

		for (int i=0;i<numTipos;i++) {
			if (strcmp(tipos[i],parentNode->Attribute("type"))==0) {
				tipoActual=i;
				break;
			}
		}

		if (tipoActual!=-1) {
			COrder* newAccion;
			
			TiXmlElement* hijo = newNode->FirstChildElement("node");
			if (hijo) {
						list<COrder*> nodosHijo;
						while (hijo && strcmp(hijo->Value(),"node")==0) {
							newAccion = createNode(tipos[tipoActual],hijo,(string("/")+hijo->Attribute("subnode")).c_str());
							newAccion->prioridad = hijo->Attribute("priority") ? atoi(hijo->Attribute("priority")) : -1;
							if (!newAccion)
								FSLibrary::Error((string("\nNodo de tipo ")+tipos[tipoActual]+" erroneo en el arbol de acciones del jugador.").c_str());
							nodosHijo.push_back(newAccion);
							TiXmlElement* enlace = hijo->FirstChildElement("link");
							while (enlace && enlace->Attribute("ref") && enlace->Attribute("event")) {
								string ref(enlace->Attribute("ref"));
								if (enlace->Attribute("subnode"))
									ref = ref+"/"+enlace->Attribute("subnode");
								string key="0";
								if (enlace->Attribute("key"))
									key=enlace->Attribute("key");
								newAccion->enlaces.push_back(CEnlace(ref,key,string(enlace->Attribute("event"))));
								enlace = enlace->NextSiblingElement();
							}
							enlace = hijo->FirstChildElement("family");
							while (enlace && strcmp(enlace->Value(),"family")==0 && enlace->Attribute("id") && enlace->Attribute("member") ) {
								string familyName(enlace->Attribute("id"));
								int miembro = atoi(enlace->Attribute("member"));

								if (mapaDeFamilias.find(familyName)!=mapaDeFamilias.end() && (mapaDeFamilias[familyName]).find(miembro)!= (mapaDeFamilias[familyName]).end())
									FSLibrary::Error("En cada familia de acciones, no puede haber más de una acción con el mismo puesto de miembro.");

								(mapaDeFamilias[familyName])[miembro]=newAccion->nodo;
								enlace = enlace->NextSiblingElement();
							}

							hijo = hijo->NextSiblingElement();
						}
						while (hijo && strcmp(hijo->Value(),"link")==0 && hijo->Attribute("ref") && hijo->Attribute("event") ) {
							string ref(hijo->Attribute("ref"));
							if (hijo->Attribute("subnode"))
								ref = ref+ "/"+hijo->Attribute("subnode");
							string key="0";
							if (hijo->Attribute("key"))
								key=hijo->Attribute("key");
							bool ignoreself=hijo->Attribute("filter") && strcmp(hijo->Attribute("filter"),"ignoreself")==0;
							for (list<COrder*>::iterator it=nodosHijo.begin();it!=nodosHijo.end();++it) {
								if (!ignoreself || strcmp((*it)->identidad.c_str(),ref.c_str())!=0)
									(*it)->enlaces.push_back(CEnlace(ref,key,string(hijo->Attribute("event"))));
							}
							hijo = hijo->NextSiblingElement();
						}
						for (list<COrder*>::iterator it=nodosHijo.begin();it!=nodosHijo.end();++it)
							nodos.push_back(*it);
						nodosHijo.clear();
			} else {		
						newAccion = createNode(tipos[tipoActual],newNode);		
						newAccion->prioridad = newNode->Attribute("priority") ? atoi(newNode->Attribute("priority")) : -1;
						if (!newAccion)
							FSLibrary::Error((string("\nNodo de tipo ")+tipos[tipoActual]+" erroneo en el arbol de acciones del jugador.").c_str());
						TiXmlElement* hijo = newNode->FirstChildElement("link");
						while (hijo && strcmp(hijo->Value(),"link")==0 && hijo->Attribute("ref") && hijo->Attribute("event")) {
							string ref(hijo->Attribute("ref"));
							if (hijo->Attribute("subnode"))
								ref = ref+ "/"+hijo->Attribute("subnode");
							string key="0";
							if (hijo->Attribute("key"))
								key=hijo->Attribute("key");
							bool ignoreself=hijo->Attribute("filter") && strcmp(hijo->Attribute("filter"),"ignoreself")==0;
							if (!ignoreself || strcmp(newAccion->identidad.c_str(),ref.c_str())!=0)
								newAccion->enlaces.push_back(CEnlace(ref,key,string(hijo->Attribute("event"))));
							hijo = hijo->NextSiblingElement();
						}
						hijo= newNode->FirstChildElement("family");
						while (hijo && hijo->Attribute("id") && hijo->Attribute("member") ) {
							string familyName(hijo->Attribute("id"));
							int miembro = atoi(hijo->Attribute("member"));

							if (mapaDeFamilias.find(familyName)!=mapaDeFamilias.end() && (mapaDeFamilias[familyName]).find(miembro)!= (mapaDeFamilias[familyName]).end())
								FSLibrary::Error("En cada familia de acciones, no puede haber más de una acción con el mismo puesto de miembro.");

							(mapaDeFamilias[familyName])[miembro]=newAccion->nodo;
							hijo = hijo->NextSiblingElement();
						}
						nodos.push_back(newAccion);
			}
			tipos[tipoActual]="restringed";
			return true;
			
		} else {
			FSLibrary::Error(string("Tipo de nodo ya usado o indefinido... type:")+parentNode->Attribute("type"));
		}

	}
	return false;
}

bool CActionGraph::add(TiXmlNode *newNode) {

	return (add(newNode->ToElement()));
}

int CActionGraph::getKey(const char* c) {
	int ret=-1;
	if (strcmp(c,"UP")==0) {
		ret = 0;
	} else if (strcmp(c,"DOWN")==0) {
		ret = 1;
	} else if (strcmp(c,"LEFT")==0) {
		ret = 2;
	} else if (strcmp(c,"RIGHT")==0) {
		ret = 3;
	} else if (strcmp(c,"B1")==0) {
		ret = 4;
	} else if (strcmp(c,"B2")==0) {
		ret = 5;
	} else if (strcmp(c,"B3")==0) {
		ret = 6;
	} else if (strcmp(c,"B4")==0) {
		ret = 7;
	} 
	return ret;
}

CAction* CActionGraph::search(const char *c) {
	for (list<COrder*>::iterator it=nodos.begin();it!=nodos.end();++it) {
		if (strcmp(c,(*it)->identidad.c_str())==0)
			return (*it)->nodo;
	}
	return NULL;
}

bool CActionGraph::mix() {
#ifdef LOG_ACCIONES
	int count=1;
	printf("\nNodos Accion: %d\n\n",nodos.size());
	for (list<COrder*>::iterator it=nodos.begin();it!=nodos.end();++it) {
		printf("%d. nombre: %s\n",count,(*it)->identidad.c_str());
		for (list<CEnlace>::iterator kt=(*it)->enlaces.begin();kt!=(*it)->enlaces.end();++kt) {
			printf("\te: %s %s %s\n",(*kt).evento.c_str(),(*kt).tecla.c_str(),(*kt).referencia.c_str());
		}
		printf("\n");
		count++;
	}
#endif
	// Enlazamiento entre acciones.
	for (list<COrder*>::iterator it=nodos.begin();it!=nodos.end();++it) {		// Por cada acción
		for (list<CEnlace>::iterator kt=(*it)->enlaces.begin();kt!=(*it)->enlaces.end();++kt) {	// Recorremos todos sus enlaces
			if (strcmp((*kt).evento.c_str(),"onkeydown")==0) { // Si es un enlace de tipo onkeydown...
				int tecla = getKey((*kt).tecla.c_str());	// Miramos que tecla tiene (lugar que ocuparía en el array de keydowns de la acción)
				
				CAction* n = (CAction*)search((*kt).referencia.c_str());	// Obtenemos la acción que corresponde a dicha referencia.
				if (tecla==-1 || !n) {
					printf("\n%s\n",(*kt).referencia.c_str());
					FSLibrary::Error("\nFallo en dependencias en la fase de mezcla del grafo de acciones.");
				}
				((CAction*)((*it)->nodo))->setKeydown(tecla,n);	// Enlazamos la acción referenciada a la acción de esta iteración para 'it'.
			} else if (strcmp((*kt).evento.c_str(),"onkeyup")==0) {	// Si es de tipo onkeyup... lo mismo.
				int tecla = getKey((*kt).tecla.c_str());
				CAction* n = (CAction*)search((*kt).referencia.c_str());
				if (tecla==-1 || !n) {
					printf("\n%s\n",(*kt).referencia.c_str());
					FSLibrary::Error("\nFallo en dependencias en la fase de mezcla del grafo de acciones.");
				}
				((CAction*)((*it)->nodo))->setKeyup(tecla,n);
			} else if (strcmp((*kt).evento.c_str(),"onrepeat")==0) {	// TODO : Para no liar el XML es recomendable implementarlo
				FSLibrary::Error("\nAún no se ha implementado funcionalidad para el evento 'onrepeat'.");
			}
		}
		if ((*it)->prioridad > -1) {
			((CAction*)(*it)->nodo)->setPriority((*it)->prioridad);		// Asignamos la prioridad que le toca a la acción (si no lo hacemos, será 0).
		}
	}
	vector<CAction*>* arrayArrows;

	for (TypeFamily::iterator it =mapaDeFamilias.begin();it!=mapaDeFamilias.end();++it) {
		arrayArrows = new vector<CAction*>;
		for (int i=0;i<it->second.size();i++)
			arrayArrows->push_back(NULL);
		for (memberFamily::iterator kt=it->second.begin();kt!=it->second.end();++kt) {
			(*arrayArrows)[kt->first]=kt->second;
			kt->second->setBrothers(arrayArrows);
		}
	}

	return true;
}

string CActionGraph::sinBarra(const char* c) {
	int i = 0;
	string ret="";
	while (c[i]!='/' && c[i]!='\0') {
		ret += c[i];
		i++;
	}
	return ret;
}

CAction* CActionGraph::getNeutro() {
	return ((CAction*)actNeutro->nodo);
}

bool CActionGraph::addByAttribute(const char* attrib,TiXmlElement *newNode) {
	return true;
}

bool CActionGraph::addByAttribute(const char* attrib,TiXmlNode *newNode) {
	return (addByAttribute(attrib,newNode->ToElement()));
}

bool CActionGraph::validateMoveAttributes(TiXmlElement *node) {
	return (node->Attribute("x") && node->Attribute("y") && node->Attribute("id"));
}

CGarbageCollectorOfActions* CActionGraph::createGarbageCollector() {
	CGarbageCollectorOfActions* garbage = new CGarbageCollectorOfActions();
	for (list<COrder*>::iterator it=nodos.begin();it!=nodos.end();++it)
		garbage->add((*it)->nodo);
	/*
	for (list<CAction**>::iterator it=listOfBrothers.begin();it!=listOfBrothers.end();++it)
		garbage->add((CAction**)*it);*/
	for (TypeFamily::iterator it=mapaDeFamilias.begin();it!=mapaDeFamilias.end();++it) {
		garbage->add(it->second.begin()->second->getBrothers());
	}

	return garbage;
}