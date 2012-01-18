#ifndef __COLLECTION_IMAGES__
#define __COLLECTION_IMAGES__

#include "FSSpriteSet.h"
#include <stack>
#include <string>
#include <map>

using namespace std;

typedef map<int,FSSpriteset*> SpritesetCollection;
typedef map<FSSpriteset*,int> SpritesetCount;

class FSControlImages {
private:
	SpritesetCollection set;
	SpritesetCount count;

	stack<int> lastIndexAdded;

	static FSControlImages* singleton;

	friend class FSScreen;
public:
	FSControlImages();
	~FSControlImages();

	int add(const char* name,Uint8 mode=ONLY_TEXTURE);
	int remove(Uint32 n);
	FSSpriteset* get(Uint32 n);
	int size();
	int search(const char* name);
	int search(FSSpriteset* object);
	void clear();
	int getCount(Uint32 n);
};

extern FSControlImages Img;


#endif
