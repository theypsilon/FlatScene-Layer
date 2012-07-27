#ifndef FS__SPRITESET_H__
#define FS__SPRITESET_H__

#include "Sprite.h"
#include <memory>

namespace FlatScene {

    class Spriteset {
    public:
        Spriteset(
            std::string     c    = "",
            unsigned char   mode = ONLY_TEXTURE
        );
        Spriteset(const Spriteset& rhs);
        Spriteset(Spriteset&& rhs);

        ~Spriteset();

        Spriteset& operator=(const Spriteset& rhs);
        Spriteset& operator=(Spriteset&& rhs);
        
        void                    swap(Spriteset& rhs);
        bool                    isNull () const { return nullptr == _impl; };
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
    template<> void swap(FlatScene::Spriteset& lhs, FlatScene::Spriteset& rhs) { lhs.swap(rhs); }
} // std

#endif // FS__SPRITESET_H__
