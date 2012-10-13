#include "SpriteSetResource.h"

namespace FlatScene {

    Spriteset::Spriteset(std::string c) 
        : ResourceHandler (new SpritesetResource(std::move(c)))
    {}

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
