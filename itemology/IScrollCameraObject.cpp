#include "IScrollCameraObject.h"


IScrollCameraObject::IScrollCameraObject( 
    const ObjectCollection& objects, Actor* target, flatscene::Rectangle* area
)   : Camera(target, area) , _objects(objects) 
{}


IScrollCameraObject::~IScrollCameraObject()
{}

int IScrollCameraObject::refresh() {
    return EXITO; 
}
