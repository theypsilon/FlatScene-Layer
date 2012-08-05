#include "SpriteSetResource.h"
#include "EventBridge.h"

namespace FlatScene {

	template class RefCountMemoryPolicy<SpritesetResource>;
	template class RefCountMemoryPolicy<SpriteResource>;
	template class RefCountMemoryPolicy<CanvasResource>;
    template class RefCountMemoryPolicy<EventBridge>;

} // FlatScene