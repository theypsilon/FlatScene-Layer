#include "RutinaColision.h"
#include "Map.h"

CRutinaColision* CRutinaColision::collisionFactory(int i) {
	CRutinaColision* ret = NULL;
	if (i==0)
		ret = new CBasicCollision0();

	return ret;
}

bool CBasicCollision0::run(int x, int y, int z, CActorScrollMap* mobil) {
	RectArea* rA = mobil->getSprite()->getArea(0);
	CMap* map = (CMap*)mobil->getUniverse();
	if (rA) {
		RectArea::iterator beginMobil = rA->begin();
		RectArea::iterator finMobil = rA->end();
		for (RectArea::iterator it = beginMobil;it!=finMobil;++it) {
			CRectangle* rect = *it;
			for (int initialx=x+rect->X(),maxx=x+rect->W();initialx <= maxx;initialx++) {
				if (map->getPixel(initialx,y,z)==2575) {
					return true;
				}
			}
			for (int initialy=y+rect->Y(),maxy=y+rect->H();initialy <= maxy;initialy++) {
				if (map->getPixel(x,initialy,z)==2575) {
					return true;
				}
			}
		}

		int Mx = x / map->getTileW();
		int My = y / map->getTileH();
		if (Mx > 0 && Mx < (map->getW()-1) && My > 0 && My< (map->getH()-1)) {
			CRectangle r1;
			CRectangle r2;
			for (int i=-1;i<2;i++) {
				for (int j=-1;j<2;j++) {
					for (ActorScrollCollection::iterator itA = map->MA[z][Mx+i][My+j]->begin(),finA = map->MA[z][Mx+i][My+j]->end();itA!=finA;++itA) {
						CActorScrollMap* obs = *itA;
						if (obs!=mobil) {

							for (RectArea::iterator itR1 = beginMobil; itR1 != finMobil; ++itR1) {
								r1 = **itR1;
								r1.X()+=x;
								r1.W()+=x;
								r1.Y()+=y;
								r1.H()+=y;
								for (RectArea::iterator itR2 = obs->getSprite()->getArea(0)->begin(),finObs = obs->getSprite()->getArea(0)->end(); itR2!=finObs;++itR2) {
									r2 = **itR2;
									r2.X()+=obs->m_Scrollxy.X();
									r2.W()+=obs->m_Scrollxy.X();
									r2.Y()+=obs->m_Scrollxy.Y();
									r2.H()+=obs->m_Scrollxy.Y();
									if (r2.Intersect(r1))
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