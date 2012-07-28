#include "ImagesPrivate.h"
#include "SpriteSetResource.h"

namespace FlatScene {

    template class RefCountMemoryPolicy<SpritesetResource>;

    Spriteset::Spriteset(std::string c, unsigned char mode) 
        : ResourceHandler (ImagesPrivate::I().add(c,mode))
    {}

    Uint8 Spriteset::getMode() const {
        return getRes().getMode();
    }

    const std::string& Spriteset::getName() const {
        return getRes().getName();
    }

    const Sprite* Spriteset::get ( unsigned int n ) const {
        return getRes().get(n);
    }

    int Spriteset::size() const {
        return getRes().size();
    }

} // flatscene
