#pragma once

#include "IScrollCamera.h"

class IScrollCameraObject : public Cinema::Camera {
    typedef std::vector<
            std::vector<
            std::list  <IScrollObject*>>>   ObjectCollection;
public:
    IScrollCameraObject (const ObjectCollection& objects,Cinema::Actor* target,FlatScene::Rectangle* area);
    ~IScrollCameraObject();

    virtual void             refresh();
private:
    const ObjectCollection& _objects;
};

