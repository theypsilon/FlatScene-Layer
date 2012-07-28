#ifndef _I_SCROLL_CAMERA_TILE_
#define _I_SCROLL_CAMERA_TILE_

#include "IScrollCamera.h"

class IScrollCameraTile :
    public Camera
{
private:
    int pix_w;
    int pix_h;

    int tiles_w;
    int tiles_h;

    int tam_w;
    int tam_h;

    const std::vector<std::vector<Tile>>& tiles;
public:
    IScrollCameraTile(const std::vector<std::vector<Tile>>& tiles,Actor* target,FlatScene::Rectangle* area);
    ~IScrollCameraTile(void);

    virtual void refresh();
};

#endif
