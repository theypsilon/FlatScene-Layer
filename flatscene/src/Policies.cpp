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


    bool CanvasMemoryPolicy::isSame(CanvasMemoryPolicy::Holder lhs, CanvasMemoryPolicy::Holder rhs) {
        return lhs == rhs;
    }

    CanvasMemoryPolicy::Holder CanvasMemoryPolicy::clone(CanvasMemoryPolicy::Holder res) {
        return ImagesPrivate::I().addSprite(res);
    }

    void CanvasMemoryPolicy::reset(CanvasMemoryPolicy::Holder& res,CanvasMemoryPolicy::Holder newval) {
        destroy(res);
        res = newval;
    }

    void CanvasMemoryPolicy::destroy(CanvasMemoryPolicy::Holder res) {
        ImagesPrivate::I().removeSprite(res);
    }

} // FlatScene