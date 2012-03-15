#include "IScrollCameraObject.h"


IScrollCameraObject::IScrollCameraObject( IScrollObjectCollection*** objects,Actor* target,Rectangle* area) 
: Camera(target,area) , objects(objects) {

	if (!objects)
		FSLib.Error("Mala definici�n de objectos");
}


IScrollCameraObject::~IScrollCameraObject(void)
{
}

int IScrollCameraObject::refresh() {




	return EXITO; 

}