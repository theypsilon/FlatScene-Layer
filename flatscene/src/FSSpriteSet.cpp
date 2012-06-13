#include "FSImagesPrivate.h"
#include "FSSpriteSetImpl.h"

namespace flatscene {

    Spriteset::Spriteset(std::string c, Uint8 mode) 
        : _impl (ImagesPrivate::I().add(c,mode))
    {}

    Spriteset::Spriteset(const Spriteset& rhs)
        : _impl (ImagesPrivate::I().add(rhs.getName(),rhs.getMode()))
    {}

    Spriteset::Spriteset(Spriteset&& rhs) {
        std::swap(rhs._impl,_impl);
    }

    Spriteset& Spriteset::operator=(const Spriteset& rhs) {
        if (&rhs == this || (rhs.getName() == getName() && rhs.getMode() == getMode()))
            return *this;

        auto temp = _impl;
        _impl = ImagesPrivate::I().add(rhs.getName(),rhs.getMode());
        ImagesPrivate::I().remove(temp);
    }

    Spriteset& Spriteset::operator=(Spriteset&& rhs) {
        std::swap(rhs._impl,_impl);
        return *this;
    }

    Spriteset::~Spriteset() {
        if (_impl)
            ImagesPrivate::I().remove(_impl);
    }

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
