#pragma once

#include "IScrollCamera.h"

class IScrollCameraObject :
	public Camera
{
private:
	IScrollObjectCollection*** objects;
public:
	IScrollCameraObject(IScrollObjectCollection*** objects,Actor* target,Rectangle* area);
	~IScrollCameraObject(void);

	int refresh();
};

