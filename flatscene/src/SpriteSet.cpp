#include "ImagesPrivate.h"
#include "SpriteSetImpl.h"

namespace std {
    template<> void swap(FlatScene::Spriteset& lhs, FlatScene::Spriteset& rhs) { 
        lhs.swap(rhs); 
    }
} // std;

namespace FlatScene {

    void doTheRemove(SpritesetResource* impl);

    Spriteset::Spriteset(std::string c, unsigned char mode) 
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

    void doTheRemove(SpritesetResource* impl) {
        if (impl)
            ImagesPrivate::I().remove(impl);
    }

    void Spriteset::swap(Spriteset& rhs) {
        auto temp = _impl;
        _impl = rhs._impl;
        rhs._impl = temp;
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
