#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "FSMessageHandler.h"
#include "FSSpriteSet.h"
#include <string>
#include <vector>

class CAnimation {
private:
    static void aliasFileAnimation(std::string& file,const char* owner);
protected:
    void cloneThisInto(CAnimation* clon);
    void generateAnimationByXML(TiXmlElement* newNode, const char* owner);
public:
    CAnimation();
    ~CAnimation();

    int nextStep();
    void reinicia();

    int paso;
    int seq;

    int num;
    int* flags;
    int* limitofsequence;
    int** sequence;
    int* sptset;
};

#endif

