#ifndef __COLLECTION_IMAGES__
#define __COLLECTION_IMAGES__

#include "FSSpriteSet.h"
#include <stack>
#include <string>
#include <map>

using namespace std;

typedef map<int,FSSpriteset*> SpritesetCollection;
typedef map<FSSpriteset*,int> SpritesetCount;

class FSImages {
public:
    FSImages();
    ~FSImages();

    int add(const char* name,Uint8 mode=ONLY_TEXTURE);
    int remove(Uint32 n);
    FSSpriteset* get(Uint32 n);
    int size();
    int search(const char* name);
    int search(FSSpriteset* object);
    void clear();
    int getCount(Uint32 n);

private:
    struct ImagesImpl;
    ImagesImpl* _impl;
    friend class FSScreen;
};

extern FSImages Img;


#endif
