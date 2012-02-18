#ifndef _I_SCROLL_CAMERA_TILE_
#define _I_SCROLL_CAMERA_TILE_

#include "iscrollcamera.h"

class IScrollCameraTile :
	public CCamera
{
private:
	int pix_w;
	int pix_h;

	int tiles_w;
	int tiles_h;

	int tam_w;
	int tam_h;

	TileBG** tiles;
	int tileset;
public:
	IScrollCameraTile(TileBG** tiles,CActor* target,CRectangle* area,CMessageHandler* pmhParent=NULL);
	~IScrollCameraTile(void);

	int refresh();
};

#endif