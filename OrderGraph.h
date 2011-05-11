#ifndef __ORDERGRAPH__
#define __ORDERGRAPH__

#include "FSMessageHandler.h"
#include "GarbageCollector.h"
#include "FSparserXML.h" 
#include <list>

#include "actions.h"

using namespace std;


typedef map<int,CAction*> memberFamily;
typedef map<string,memberFamily> TypeFamily;

class CEnlace {
public:
	CEnlace(string referencia,string tecla,string evento);
	~CEnlace();
	string referencia;
	string tecla;
	string evento;
};

class COrder {
public:

	COrder(CAction* n, string identidad="");
	~COrder();

	CAction* nodo;
	int prioridad;
	string identidad;
	list<CEnlace> enlaces;
};

class COrderGraph {
private:
	CAction* actual;

public:

	COrderGraph();
	virtual ~COrderGraph();

	virtual bool addByAttribute(const char* attrib,TiXmlElement* newNode)=0;
	virtual bool addByAttribute(const char* attrib,TiXmlNode* newNode)=0;
	virtual bool add(TiXmlElement* newNode)=0;
	virtual bool add(TiXmlNode* newNode)=0;
	virtual bool mix();
	//virtual inline CAction* get();

	//virtual bool isComplete()=0;

};

#include "Action.h"
class CActorScrollMap;

class CActionGraph : public COrderGraph {
private:
	list<COrder*> nodos;
	COrder* actNeutro;

	TypeFamily mapaDeFamilias;
	
	const char** tipos ;
	int numTipos;	

	bool validateMoveAttributes(TiXmlElement* node);
	COrder* createNode(const char* c,TiXmlElement* newNode, const char* s=NULL);
	int getKey(const char* c);
	CAction* search(const char* c);
	string sinBarra(const char* c);

	CActorScrollMap* player;
public:

	CActionGraph(CActorScrollMap* pmhParent);
	virtual ~CActionGraph();

	bool addByAttribute(const char* attrib,TiXmlElement* newNode);
	bool addByAttribute(const char* attrib,TiXmlNode* newNode);
	bool add(TiXmlElement* newNode);
	bool add(TiXmlNode* newNode);
	bool mix();
	CAction* getNeutro();


	//inline CAction* get();

	//bool isComplete();

	CGarbageCollectorOfActions* createGarbageCollector();

};

#endif