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
        Spriteset(const Spriteset& rhs);
        Spriteset(Spriteset&& rhs);

        ~Spriteset();

        Spriteset& operator=(const Spriteset& rhs);
        Spriteset& operator=(Spriteset&& rhs);
        
        void                    swap(Spriteset& rhs);
        bool                    isNull () const; // Only may happen if the object has been moved to another
        // Following methods yield Undefined Behavior when IsNull returns true, aka the object has been moved

        const Sprite*           get    ( unsigned int n ) const;
        int                     size   () const;
        const std::string&      getName() const;
        unsigned char           getMode() const;

    private :
        struct SpritesetImpl;
        SpritesetImpl*          _impl;
        friend void             doTheRemove(SpritesetImpl* impl);

        friend class Screen;
        friend class Images;
        friend class ImagesPrivate;
    };

} // flatscene

namespace std {
    template<> void swap(flatscene::Spriteset& lhs, flatscene::Spriteset& rhs);
} // std

#endif // FS__SPRITESET_H__
