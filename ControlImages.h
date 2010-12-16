#ifndef __COLLECTION_IMAGES__
#define __COLLECTION_IMAGES__

#include "SpriteSet.h"
#include <stack>
#include <string>
#include <map>

using namespace std;

typedef map<int,CSpriteset*> SpritesetCollection;
typedef map<CSpriteset*,int> SpritesetCount;

class CControlImages {
private:
	SpritesetCollection set;
	SpritesetCount count;

	stack<int> lastIndexAdded;

	static CControlImages* singleton;

	friend class CScreen;
public:
	CControlImages();
	~CControlImages();

	int add(const char* name,Uint8 mode=ONLY_TEXTURE);
	int remove(Uint32 n);
	CSpriteset* get(Uint32 n);
	int size();
	int search(const char* name);
	int search(CSpriteset* object);
	void clear();
	int getCount(Uint32 n);
};

extern CControlImages CImg;


#endif