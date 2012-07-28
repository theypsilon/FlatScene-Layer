#include "IScrollCameraTile.h"


IScrollCameraTile::IScrollCameraTile(
    const std::vector<std::vector<Tile>>& tiles, Actor* target,FlatScene::Rectangle* area
)  // Se ha de llamar a la clase Base para una correcta inicializaci�n
    : Camera(target,area) , tiles(tiles){

        IScrollLevel * uni = dynamic_cast<IScrollLevel*>(target->getUniverse());

        if (!uni) {

            throw Exception("CameraTile : Mala definici�n del nivel");

        } else {

            tiles_h = uni->getH();
            tiles_w = uni->getW();

            pix_w = uni->getTileW();
            pix_h = uni->getTileH();

            Float aux_w=floor(((Float)area->getW())/((Float)pix_w)/1.0 + 0.9);
            Float aux_h=floor(((Float)area->getH())/((Float)pix_h)/1.0 + 0.9);

            tam_w=  ((int)aux_w)+1;
            tam_h= ((int)aux_h)+1;

        }
}


IScrollCameraTile::~IScrollCameraTile() {}


void IScrollCameraTile::refresh() {

    Point p;

    int actor_x = target->renderPoint.x;
    int actor_y = target->renderPoint.y;

    translate(-actor_x % pix_w ,-actor_y % pix_h,0);

    int x = actor_x / pix_w;
    int y = actor_y / pix_h;

    for (int i=0;i<tam_w;i++) {
        for (int j=0;j<tam_h;j++) {
            const Tile& ind =  tiles.at((j+y)%tiles_h).at((i+x)%tiles_w);
            auto graph = ind.getGraph();
            if (graph) {
                p.x=i*pix_w;
                p.y=j*pix_h;

                if (ind.getFlags() & 0x001) {
                    p.x+=pix_w;
                }

                if (ind.getFlags() & 0x010) {
                    p.y+=pix_h;
                }

                graph->put(p,(unsigned char) ind.getFlags());
            } 
        }
    }
}
