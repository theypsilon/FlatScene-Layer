#ifndef FS__SPRITESET_H__
#define FS__SPRITESET_H__

#include "FSSprite.h"
#include <memory>

namespace flatscene {

    class Spriteset {
    public:
        Spriteset(
            std::string c    = "",
            Uint8       mode = ONLY_TEXTURE
        );

        const Sprite*           get    ( unsigned int n ) const;
        int                     size   () const;
        const std::string&      getName() const;
        unsigned char           getMode() const;
    protected:
        ~Spriteset();
    private :
        struct SpritesetImpl;
        SpritesetImpl* const    _impl;

        friend class Screen;
        friend class Images;
        friend class ImagesPrivate;
    };

} // flatscene

#endif // FS__SPRITESET_H__
