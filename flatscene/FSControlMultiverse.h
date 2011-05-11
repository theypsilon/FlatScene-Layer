#ifndef __C_COLLECTION_M__
#define __C_COLLECTION_M__

#include "FSUniverse.h"
#include "FSMessageHandler.h"
#include <map>
#include <string>

typedef  std::list<CUniverse*> UniverseCollection;
typedef map<CMessageHandler*,UniverseCollection*> MultiverseByAdmin;

class CControlMultiverse {
private:
	CMessageHandler* admin;	
	MultiverseByAdmin session;
	UniverseCollection* unisCurrent;

	CMessageHandler* setAdmin(CMessageHandler* newAdmin);

	static CControlMultiverse* singleton;

	bool working;

	friend class CUniverse;
public:
	CControlMultiverse();
	~CControlMultiverse();

	CUniverse* add(CUniverse* uni,Uint8 slot=0);
	CUniverse* universeNamed(string uniName,Uint8 slot=0);
	CUniverse* universeNamed(const char* uniName,Uint8 slot=0);
	void erase(CUniverse* uniKilled);
	void clear();
	int size();

	UniverseCollection::iterator begin();
	UniverseCollection::iterator end();
};

extern CControlMultiverse CMultiverse;

#endif