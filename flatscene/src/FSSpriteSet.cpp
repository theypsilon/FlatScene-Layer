#include "FSSpriteSetImpl.h"

namespace flatscene {

    Spriteset::Spriteset(std::string c, Uint8 mode) 
        : _impl (new SpritesetImpl(c,mode)) {}

    Spriteset::~Spriteset() {}

    Uint8 Spriteset::getMode() const {
        return _impl->getMode();
    }

    const std::string& Spriteset::getName() const {
        return _impl->getName();
    }

    const Sprite* Spriteset::get ( unsigned int n ) const {
        return _impl->get(n);
    }

    int Spriteset::size() const {
        return _impl->size();
    }

} // flatscene
