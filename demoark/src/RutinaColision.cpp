#include "RutinaColision.h"
#include "Map.h"
#include "FSLibrary.h"

CRutinaColision* CRutinaColision::collisionFactory(int i) {
	CRutinaColision* ret = NULL;
	if (i==0)
		ret = new CBasicCollision0();

	return ret;
}

bool CBasicCollision0::run(int x, int y, int z, CActorScrollMap* mobil) {
	RectArea* rA = mobil->getSprite()->getArea(0);
	CMap* map = (CMap*)mobil->getUniverse();

	//Write.erase( Write.line(0,mobil->renderPoint.x,mobil->renderPoint.y,"color : #%06X",map->getPixel(x,y,z)), true);

	if (rA) {
		RectArea::iterator beginMobil = rA->begin();
		RectArea::iterator finMobil = rA->end();
		for (RectArea::iterator it = beginMobil;it!=finMobil;++it) {
			FSRectangle* rect = *it;
			for (int initialx=x+rect->x,maxx=x+rect->w;initialx <= maxx;initialx++) {
				if (map->getPixel(initialx,y,z)==0xFFA800F8) {
					return true;
				}
			}
			for (int initialy=y+rect->y,maxy=y+rect->h;initialy <= maxy;initialy++) {
				if (map->getPixel(x,initialy,z)==0xFFA800F8) {
					return true;
				}
			}
		}

		int Mx = x / map->getTileW();
		int My = y / map->getTileH();
		if (Mx > 0 && Mx < (map->getW()-1) && My > 0 && My< (map->getH()-1)) {
			FSRectangle r1;
			FSRectangle r2;
			for (int i=-1;i<2;i++) {
				for (int j=-1;j<2;j++) {
					for (ActorScrollCollection::iterator itA = map->MA[z][Mx+i][My+j]->begin(),finA = map->MA[z][Mx+i][My+j]->end();itA!=finA;++itA) {
						CActorScrollMap* obs = *itA;
						if (obs!=mobil) {

							for (RectArea::iterator itR1 = beginMobil; itR1 != finMobil; ++itR1) {
								r1 = **itR1;
								r1.x+=x;
								r1.w+=x;
								r1.y+=y;
								r1.h+=y;
								for (RectArea::iterator itR2 = obs->getSprite()->getArea(0)->begin(),finObs = obs->getSprite()->getArea(0)->end(); itR2!=finObs;++itR2) {
									r2 = **itR2;
									r2.x+=obs->m_Scrollxy.x;
									r2.w+=obs->m_Scrollxy.x;
									r2.y+=obs->m_Scrollxy.y;
									r2.h+=obs->m_Scrollxy.y;
									if (r2.intersect(r1))
										return true;
								}
							}

						}

					}
				}
			}
		}
	}
	return false;
}
