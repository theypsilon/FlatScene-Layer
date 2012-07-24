#include "IScrollCameraObject.h"


IScrollCameraObject::IScrollCameraObject( 
    const ObjectCollection& objects, Actor* target, FlatScene::Rectangle* area
)   : Camera(target, area) , _objects(objects) 
{}


IScrollCameraObject::~IScrollCameraObject()
{}

void IScrollCameraObject::refresh()
{}
