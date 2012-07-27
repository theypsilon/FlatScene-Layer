#include "SpriteSetResource.h"
#include "ImagesPrivate.h"

namespace FlatScene {

    bool SpritesetMemoryPolicy::isSame(SpritesetMemoryPolicy::Holder lhs, SpritesetMemoryPolicy::Holder rhs) {
        return lhs == rhs;
    }

    SpritesetMemoryPolicy::Holder SpritesetMemoryPolicy::clone(SpritesetMemoryPolicy::Holder res) {
        return ImagesPrivate::I().add(res->getName(),res->getMode());
    }

    void SpritesetMemoryPolicy::reset(SpritesetMemoryPolicy::Holder& res,SpritesetMemoryPolicy::Holder newval) {
        destroy(res);
        res = newval;
    }

    void SpritesetMemoryPolicy::destroy(SpritesetMemoryPolicy::Holder res) {
        ImagesPrivate::I().remove(res);
    }

    Spriteset::Spriteset(std::string c, unsigned char mode) 
        : ResourceHandler (ImagesPrivate::I().add(c,mode))
    {}

} // FlatScene