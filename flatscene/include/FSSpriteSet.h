#ifndef FS__SPRITESET_H__
#define FS__SPRITESET_H__

#include "FSSprite.h"
#include <memory>

namespace flatscene {

    class Spriteset {
    public:

        const Sprite* get ( unsigned int n ) const;

        int size () const;

        const std::string& getName() const;

        Uint8 getMode() const;

    private :
        struct SpritesetImpl;
        SpritesetImpl* const _impl;

        Spriteset(std::string c = "",Uint8 mode=ONLY_TEXTURE);
        virtual ~Spriteset();

        friend class Screen;
        friend class Images;
        friend class ImagesPrivate;
    };

} // flatscene

#endif // FS__SPRITESET_H__
