#ifndef FS__SPRITESET_H__
#define FS__SPRITESET_H__

#include "Sprite.h"
#include "ResourceHandler.h"

namespace FlatScene {

    class SpritesetResource;

    class Spriteset 
    : public ResourceHandler<SpritesetResource,RefCountMemoryPolicy<SpritesetResource> > {
    public:
        Spriteset(std::string c    = "");
        
        bool                    isNull () const { return nullptr == &getRes(); };
        
        /* Following methods yield Undefined Behavior when IsNull returns true, *
         * aka the object has been moved                                        */

        const Sprite*           get    ( unsigned int n ) const;
        int                     size   () const;
        const std::string&      getName() const;

    private :
        friend class Screen;
        friend class Images;
        friend class ImagesPrivate;
    };

} // flatscene

#endif // FS__SPRITESET_H__
