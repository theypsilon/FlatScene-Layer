#include "ImageSetResource.h"
#include "EventBridge.h"

namespace FlatScene {

	template class RefCountMemoryPolicy<ImageSetResource<Sprite>>;
    template class RefCountMemoryPolicy<ImageSetResource<Canvas>>;
    template class RefCountMemoryPolicy<ImageSetResource<SoftwareSprite>>;
    template class RefCountMemoryPolicy<ImageSetResource<SoftwareCanvas>>;
	template class RefCountMemoryPolicy<CanvasResource>;
    template class RefCountMemoryPolicy<EventBridge>;

} // FlatScene