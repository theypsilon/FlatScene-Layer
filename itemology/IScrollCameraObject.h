#pragma once
#include "iscrollcamera.h"
class IScrollCameraObject :
	public FSCamera
{
private:
	IScrollObjectCollection*** objects;
public:
	IScrollCameraObject(IScrollObjectCollection*** objects,FSActor* target,FSRectangle* area);
	~IScrollCameraObject(void);

	int refresh();
};

