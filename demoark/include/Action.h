#ifndef __ACTION_H__
#define __ACTION_H__

#include "FSImages.h"
#include "Animation.h"
#include "FSdefinitions.h"
#include "Node.h"
#include "FSTime.h"
#include "Animation.h"
#include <string>
#include <stdio.h>
#include <list>
#include <typeinfo>
#include "FSMessageHandler.h"
using namespace std;


class CActorScrollMap;
class CAction;

typedef list<CAction*> ActionCollection;

class CAction : public CNode, public CAnimation
{
private:
	CAction* actionKeydown[NUM_PJKEY];
	CAction* actionKeyup[NUM_PJKEY];
	CAction* predecesor;
	CAction* sucesor;
	vector<CAction*>* actionBrothers;
	string  id;

	int priority;

	bool activo; // disponible,use,reciclar.

public:
	CActorScrollMap* executor;

	CAction(CActorScrollMap* executor,TiXmlElement* fuente,const char* i);
	virtual ~CAction();
	virtual void inDelete();

	CAction* getPredecesor();
	CAction* getSucesor();
	void setPredecesor(CAction* actPre);
	void setSucesor(CAction* actSuc);

	vector<CAction*>* getBrothers() { return actionBrothers; };
	void setBrothers(vector<CAction*>* bro) { this->actionBrothers=bro; };

	CAction* getKeydown(int n);
	CAction* getKeyup(int n);
	void setKeydown(int n, CAction* act);
	void setKeyup(int n,CAction* act);
	virtual void execute();
	virtual void firstExecute();
	virtual void lastExecute();
	const char* getId();

	// TODO. Posiblemente Prescindible.
	bool disponible() { return !activo; };
	virtual void use();
	virtual void recycle();

	void setPriority(int p) { priority=p; };
	int inline getPriority() { return priority; };

	virtual CAction* mix(ActionCollection mix);
	virtual bool elapsed(Uint32 lapso) { return true; };

	virtual CAction* clone(CActorScrollMap* exe=NULL);

	static inline bool orderReferenceNodes(CAction* x, CAction* y) {
		return (x->getPriority()>y->getPriority());
	};
};

#endif
