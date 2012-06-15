#include "FSImagesPrivate.h"
#include "FSSpriteSetImpl.h"

namespace std {
    template<> void swap(flatscene::Spriteset& lhs, flatscene::Spriteset& rhs) {
        lhs.swap(rhs);
    }
} // std

namespace flatscene {

    Spriteset::Spriteset(std::string c, Uint8 mode) 
        : _impl (ImagesPrivate::I().add(c,mode))
    {}

    Spriteset::Spriteset(const Spriteset& rhs)
        : _impl (ImagesPrivate::I().add(rhs.getName(),rhs.getMode()))
    {}

    Spriteset::Spriteset(Spriteset&& rhs) : _impl(nullptr) {
        swap(rhs);
    }

    Spriteset& Spriteset::operator=(const Spriteset& rhs) {
        if (&rhs == this || (rhs.getName() == getName() && rhs.getMode() == getMode()))
            return *this;

        auto temp = _impl;
        _impl = ImagesPrivate::I().add(rhs.getName(),rhs.getMode());
        doTheRemove(temp);
        return *this;
    }

    Spriteset& Spriteset::operator=(Spriteset&& rhs) {
        doTheRemove(_impl);
        _impl = nullptr;
        swap(rhs);
        return *this;
    }

    Spriteset::~Spriteset() {
        doTheRemove(_impl);
    }

    void doTheRemove(Spriteset::SpritesetImpl* impl) {
        if (impl)
            ImagesPrivate::I().remove(impl);
    }

    void Spriteset::swap(Spriteset& rhs) {
        auto temp = _impl;
        _impl = rhs._impl;
        rhs._impl = temp;
    }

    bool Spriteset::isNull() const {
        return nullptr == _impl;
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
