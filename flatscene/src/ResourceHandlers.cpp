#include "SpriteSetResource.h"
#include "EventBridge.h"

namespace FlatScene {

	template class RefCountMemoryPolicy<SpritesetResource<ONLY_GPU>>;
    template class RefCountMemoryPolicy<SpritesetResource<ONLY_CPU>>;
	template class RefCountMemoryPolicy<CanvasResource>;
    template class RefCountMemoryPolicy<EventBridge>;

} // FlatScene