#include "IScrollCameraObject.h"


IScrollCameraObject::IScrollCameraObject( IScrollObjectCollection*** objects,CActor* target,CRectangle* area,CMessageHandler* pmhParent) :
// Se ha de llamar a la clase Base para una correcta inicialización
CCamera(target,area,pmhParent) , objects(objects) {

	if (!objects)
		CLibrary::Error("Mala definición de objectos");
}


IScrollCameraObject::~IScrollCameraObject(void)
{
}

int IScrollCameraObject::refresh() {




	return EXITO; 

}