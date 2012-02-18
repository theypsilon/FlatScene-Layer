#pragma once
#include "iscrollcamera.h"
class IScrollCameraObject :
	public CCamera
{
private:
	IScrollObjectCollection*** objects;
public:
	IScrollCameraObject(IScrollObjectCollection*** objects,CActor* target,CRectangle* area,CMessageHandler* pmhParent=NULL);
	~IScrollCameraObject(void);

	int refresh();
};

