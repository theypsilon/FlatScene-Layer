#ifndef FS__SPRITESET_H__
#define FS__SPRITESET_H__

#include "Sprite.h"
#include "ResourceHandler.h"

namespace FlatScene {

    class SpritesetResource;

    struct SpritesetMemoryPolicy {
        typedef SpritesetResource* Holder;

        static bool isSame(Holder lhs, Holder rhs);
        static Holder clone(Holder res);
        static void destroy(Holder res);
        static void reset(Holder& res,Holder newval);
    };

    class Spriteset : public ResourceHandler<SpritesetResource,SpritesetMemoryPolicy> {
    public:
        Spriteset(
            std::string     c    = "",
            unsigned char   mode = ONLY_TEXTURE
        );
        
        bool                    isNull () const { return nullptr == &getRes(); };
        // Following methods yield Undefined Behavior when IsNull returns true, aka the object has been moved

        const Sprite*           get    ( unsigned int n ) const;
        int                     size   () const;
        const std::string&      getName() const;
        unsigned char           getMode() const;

    private :
        friend class Screen;
        friend class Images;
        friend class ImagesPrivate;
    };

} // flatscene

#endif // FS__SPRITESET_H__
