#ifndef __RUTINA_COLISION_H__
#define __RUTINA_COLISION_H__

class CActorScrollMap;

class CRutinaColision {
public:
	virtual bool run(int x,int y,int z,CActorScrollMap* mobil)=0;

	static CRutinaColision* collisionFactory(int i=0);
};

class CBasicCollision0 : public CRutinaColision {
public:
	bool run(int x,int y,int z,CActorScrollMap* mobil);
};

#endif
