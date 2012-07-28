#ifndef __COLLECTION_IMAGES__
#define __COLLECTION_IMAGES__

#include "SpriteSet.h"
#include "Singleton.h"
#include "Noncopyable.h"
#include <stack>
#include <string>
#include <map>

namespace FlatScene {

    typedef std::map<int,Spriteset*> SpritesetCollection;
    typedef std::map<Spriteset*,int> SpritesetCount;

    class Images : private Util::NonCpmvable, public Util::Singleton<Images> {
        friend class Util::Singleton<Images>;
    public:
        int add(const char* name,Uint8 mode=ONLY_TEXTURE);
        int remove(Uint32 n);
        Spriteset* get(Uint32 n);
        int size();
        int search(const char* name);
        int search(Spriteset* object);
        void clear();
        int getCount(Uint32 n);
    private:
        Images();
        ~Images();

        struct ImagesImpl;
        ImagesImpl* _impl;
        friend class Screen;
    };

    #ifdef GLOBAL_SINGLETON_REFERENCES
    extern Images& Img;
    #endif

} // flatscene

#endif
