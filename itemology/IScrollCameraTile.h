#ifndef _I_SCROLL_CAMERA_TILE_
#define _I_SCROLL_CAMERA_TILE_

#include "IScrollCamera.h"

class IScrollCameraTile :
	public FSCamera
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
	IScrollCameraTile(TileBG** tiles,FSActor* target,FSRectangle* area);
	~IScrollCameraTile(void);

	int refresh();
};

#endif
