#ifndef FS__COLLECTION_IMAGES_PRIVATE__
#define FS__COLLECTION_IMAGES_PRIVATE__

#include "SpriteSet.h"
#include "Singleton.h"
#include "Noncopyable.h"
#include <deque>
#include <string>
#include <map>

namespace FlatScene {
    
    class ImagesPrivate : private Util::NonCpmvable, public Util::Singleton<ImagesPrivate> {
        friend class Util::Singleton<ImagesPrivate>;
        typedef Spriteset::SpritesetImpl SpritesetImpl;
        typedef std::map<SpritesetImpl*,unsigned int> SpriteCounter;
    public:
        SpritesetImpl*      add(const std::string& name,unsigned char mode=ONLY_TEXTURE);
        void                remove(SpritesetImpl* sptset);
        void                clear();
        SpritesetImpl*      search(const std::string& name,unsigned char mode=ONLY_TEXTURE) const;
        std::size_t         size() const;
        unsigned int        getCount(SpritesetImpl* sptset) const;
    private:
        ImagesPrivate();
        ~ImagesPrivate();

        SpriteCounter       count;

        friend class Screen;
    };

} // flatscene

#endif // FS__COLLECTION_IMAGES_PRIVATE__
