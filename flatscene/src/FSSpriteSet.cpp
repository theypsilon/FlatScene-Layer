#include "FSSpriteSetImpl.h"

FSSpriteset::FSSpriteset(std::string c, Uint8 mode) 
: _impl (new SpritesetImpl(c,mode)) {}
FSSpriteset::~FSSpriteset() {}

Uint8 FSSpriteset::getMode() const {
    return _impl->getMode();
}

const std::string& FSSpriteset::getName() const {
    return _impl->getName();
}

bool FSSpriteset::has ( FSSprite* pspt ) const {
    return _impl->has(pspt) ;
}

int FSSpriteset::search( FSSprite* pspt ) const {
    return _impl->search(pspt);
}

FSSprite* FSSpriteset::get ( unsigned int n ) const {
    return _impl->get(n);
}

int FSSpriteset::size() const {
    return _impl->size();
}