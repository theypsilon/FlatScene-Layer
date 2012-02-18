#include "IScrollCameraObject.h"


IScrollCameraObject::IScrollCameraObject( IScrollObjectCollection*** objects,CActor* target,CRectangle* area,CMessageHandler* pmhParent) :
// Se ha de llamar a la clase Base para una correcta inicializaci�n
CCamera(target,area,pmhParent) , objects(objects) {

	if (!objects)
		CLibrary::Error("Mala definici�n de objectos");
}


IScrollCameraObject::~IScrollCameraObject(void)
{
}

int IScrollCameraObject::refresh() {




	return EXITO; 

}