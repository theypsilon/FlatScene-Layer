#include "FSControlOutputTextImpl.h"

FSControlOutputText::FSControlOutputTextImpl::FSText::FSText(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next) {
    fx = NULL;
    Box = new FSTextBox(file,text,x,y,Lim,ttf_fnt,next);
    type = TT_BOX;
}

FSControlOutputText::FSControlOutputTextImpl::FSText::FSText() {
    fx = NULL;
    Line = new SLineText;
    type = TT_LINE;
}

FSControlOutputText::FSControlOutputTextImpl::FSText::~FSText() {
    if (fx) {
        delete fx;
        fx=NULL;
    }

    if (type == TT_BOX) {
        delete Box;
        Box = NULL;
    } else if (type == TT_LINE) {
        delete Line;
        Line = NULL;
    }

}

TypeText FSControlOutputText::FSControlOutputTextImpl::FSText::Type() const {
    return type;
}
