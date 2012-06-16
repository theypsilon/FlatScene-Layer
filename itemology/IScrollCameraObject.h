#pragma once

#include "IScrollCamera.h"

class IScrollCameraObject : public Camera {
    typedef std::vector<
            std::vector<
            std::list  <IScrollObject*>>>   ObjectCollection;
public:
    IScrollCameraObject (const ObjectCollection& objects,Actor* target,flatscene::Rectangle* area);
    ~IScrollCameraObject();

    virtual int             refresh();
private:
    const ObjectCollection& _objects;
};

