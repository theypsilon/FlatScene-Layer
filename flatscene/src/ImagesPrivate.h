#ifndef FS__COLLECTION_IMAGES_PRIVATE__
#define FS__COLLECTION_IMAGES_PRIVATE__

#include "SpriteSet.h"
#include "Singleton.h"
#include "Noncopyable.h"
#include <deque>
#include <string>
#include <unordered_map>

namespace FlatScene {
    
    class ImagesPrivate : private Util::NonCpmvable, public Util::Singleton<ImagesPrivate> {
        friend class Util::Singleton<ImagesPrivate>;
        typedef std::unordered_map<SpritesetResource*,unsigned int> SpriteSetCounter;
        typedef std::unordered_map< void*,
            std::unordered_map<unsigned int, SpriteResource*>> SpriteMapper;

        typedef std::unordered_map< SpriteResource*,unsigned int> SpriteCounter;
    public:
        SpritesetResource*  add(const std::string& name,unsigned char mode=ONLY_TEXTURE);
        void                remove(SpritesetResource* sptset);
        void                clear();
        SpritesetResource*  search(const std::string& name,unsigned char mode=ONLY_TEXTURE) const;
        std::size_t         size() const;
        unsigned int        getCount(SpritesetResource* sptset) const;
        SpriteResource*     addSprite(SDL_Surface* surf,unsigned int pos, unsigned char mode=ONLY_TEXTURE);
        SpriteResource*     searchSprite(SDL_Surface* surf,unsigned int pos, unsigned char mode=ONLY_TEXTURE) const;
        void                removeSprite(SpriteResource* spt);
    private:
        ImagesPrivate();
        ~ImagesPrivate();

        SpriteSetCounter    count;
        SpriteCounter       countSprite;
        SpriteMapper        mapSprite;

        friend class Screen;
    };

} // flatscene

#endif // FS__COLLECTION_IMAGES_PRIVATE__
