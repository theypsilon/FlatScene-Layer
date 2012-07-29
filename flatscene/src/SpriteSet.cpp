#include "SpriteSetResource.h"

namespace FlatScene {

    template class RefCountMemoryPolicy<SpritesetResource>;

    Spriteset::Spriteset(std::string c, GraphicMode mode) 
        : ResourceHandler (new SpritesetResource(c,mode))
    {}

    GraphicMode Spriteset::getMode() const {
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
