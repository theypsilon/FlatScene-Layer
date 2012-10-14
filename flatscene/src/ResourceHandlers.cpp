#include "SpriteSetResource.h"
#include "EventBridge.h"

namespace FlatScene {

	template class RefCountMemoryPolicy<SpritesetResource<Sprite>>;
    //template class RefCountMemoryPolicy<SpritesetResource<Canvas>>;
	template class RefCountMemoryPolicy<CanvasResource>;
    template class RefCountMemoryPolicy<EventBridge>;

} // FlatScene