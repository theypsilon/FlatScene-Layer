#ifndef __ACTION_QUIET_AUTO_0_H__
#define __ACTION_QUIET_AUTO_0_H__

#include "ActionMove.h"

class CActionQuietAuto0 : public CActionMove
{
private:
    int lapso;
public:
    CActionQuietAuto0(CActorScrollMap* executor,TiXmlElement* fuente,const char* i);
    virtual ~CActionQuietAuto0();
    void inDelete();
    void firstExecute();
    virtual void execute();
    void lastExecute();

    CAction* clone(CActorScrollMap* exe=NULL);
};

#endif