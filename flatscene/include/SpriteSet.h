#ifndef FS__SPRITESET_H__
#define FS__SPRITESET_H__

#include "Sprite.h"
#include "ResourceHandler.h"

namespace FlatScene {

    template <GraphicMode mode> class SpritesetResource;

    class Spriteset 
    : public ResourceHandler<
        SpritesetResource<ONLY_GPU>,
        RefCountMemoryPolicy<SpritesetResource<ONLY_GPU>> > {
    public:
        Spriteset(std::string c    = "");

        const std::string&         getName() const;
        const std::vector<Sprite>& get    () const;
        const std::vector<Sprite>* const operator->() const;
    };

} // flatscene

#endif // FS__SPRITESET_H__
