#include "SpriteSetResource.h"

namespace FlatScene {

    Spriteset::Spriteset(std::string c) 
        : ResourceHandler (SpritesetResource<ONLY_GPU>::create(std::move(c)))
    {}

    const std::string& Spriteset::getName() const {
        return getRes().getName();
    }

    const std::vector<Sprite>& Spriteset::get() const {
        return getRes().get();
    }

    const std::vector<Sprite>* const Spriteset::operator->() const {
        return &getRes().get();
    }

} // flatscene
