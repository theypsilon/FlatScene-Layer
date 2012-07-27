#include "ImagesPrivate.h"
#include "SpriteSetPolicies.h"

namespace FlatScene {

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
