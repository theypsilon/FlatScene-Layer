#ifndef __C_COLLECTION_M__
#define __C_COLLECTION_M__

#include "FSUniverse.h"
#include "FSMessageHandler.h"
#include <map>
#include <string>

typedef  std::list<FSUniverse*> UniverseCollection;
typedef map<FSMessageHandler*,UniverseCollection*> MultiverseByAdmin;

class FSControlMultiverse {
private:
	FSMessageHandler* admin;	
	MultiverseByAdmin session;
	UniverseCollection* unisCurrent;

	FSMessageHandler* setAdmin(FSMessageHandler* newAdmin);

	static FSControlMultiverse* singleton;

	bool working;

	friend class FSUniverse;
public:
	FSControlMultiverse();
	~FSControlMultiverse();

	FSUniverse* add(FSUniverse* uni,Uint8 slot=0);
	FSUniverse* universeNamed(string uniName,Uint8 slot=0);
	FSUniverse* universeNamed(const char* uniName,Uint8 slot=0);
	void erase(FSUniverse* uniKilled);
	void clear();
	int size();

	UniverseCollection::iterator begin();
	UniverseCollection::iterator end();
};

extern FSControlMultiverse FSMultiverse;

#endif
