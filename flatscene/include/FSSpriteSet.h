#ifndef __SPRITESET_H__
#define __SPRITESET_H__

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
        const std::unique_ptr<SpritesetImpl> _impl;

        Spriteset(std::string c = "",Uint8 mode=ONLY_TEXTURE);
        virtual ~Spriteset();

        friend class Screen;
        friend class Images;
    };

} // flatscene

#endif
