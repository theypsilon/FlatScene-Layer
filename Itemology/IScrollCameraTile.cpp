#include "IScrollCameraTile.h"


IScrollCameraTile::IScrollCameraTile(TileBG** tiles, CActor* target,CRectangle* area,CMessageHandler* pmhParent) :
// Se ha de llamar a la clase Base para una correcta inicialización
CCamera(target,area,pmhParent) , tiles(tiles){

	if (!tiles) 
		CLibrary::Error("CameraTile : Mala definición de tiles");

	IScrollLevel * uni = dynamic_cast<IScrollLevel*>(target->getUniverse());

	if (!uni) {

		CLibrary::Error("CameraTile : Mala definición del nivel");
		tileset = -1;

	} else {

		tiles_h = uni->getH();
		tiles_w = uni->getW();

		pix_w = uni->getTileW();
		pix_h = uni->getTileH();

		float aux_w=floor(((float)area->getW())/((float)pix_w)/1.0 + 0.9);
		float aux_h=floor(((float)area->getH())/((float)pix_h)/1.0 + 0.9);

		tam_w=  ((int)aux_w)+1;
		tam_h= ((int)aux_h)+1;

		tileset = uni->getTileset();

	}
}


IScrollCameraTile::~IScrollCameraTile(void)
{
}


int IScrollCameraTile::refresh() {

	if (tileset == -1)
		return FRACASO;

	CPoint p;

	int actor_x = target->renderPoint.X();
	int actor_y = target->renderPoint.Y();

	translate(-actor_x % pix_w ,-actor_y % pix_h,0);

	int x = actor_x / pix_w;
	int y = actor_y / pix_h;

	for (int i=0;i<tam_w;i++) {
		for (int j=0;j<tam_h;j++) {
			TileBG ind =  tiles[(j+y)%tiles_h][(i+x)%tiles_w];
			if (ind.graph>0) {
				ind.graph--;
				p.X()=i*pix_w;
				p.Y()=j*pix_h;

				if (ind.flags & 0x001) {
					p.X()+=pix_w;
				}

				if (ind.flags & 0x010) {
					p.Y()+=pix_h;
				}

				CImg.get(tileset+ind.fileGraph)->get(ind.graph)->put(p,ind.flags);
			} 
		}
	}

	return EXITO; 

}