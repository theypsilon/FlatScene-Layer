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
        typedef std::unordered_map< CanvasResource*,unsigned int> SpriteCounter;
    public:
        SpritesetResource*  add(const std::string& name,unsigned char mode=ONLY_TEXTURE);
        void                clear();
        CanvasResource*     addSprite();
    private:
        SpriteSetCounter    count;
        SpriteCounter       countSprite;

        friend class Screen;
    };

} // flatscene

#endif // FS__COLLECTION_IMAGES_PRIVATE__
