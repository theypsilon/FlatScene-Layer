#include "FSWriterImpl.h"

FSWriter::WriterImpl::FSText::FSText(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next) 
    : fx(nullptr)
    , Box(new FSTextBox(file,text,x,y,Lim,ttf_fnt,next))
    , type(TT_BOX)
{}

FSWriter::WriterImpl::FSText::FSText() 
    : fx(nullptr)
    , Line(new SLineText)
    , type(TT_LINE)
{}

FSWriter::WriterImpl::FSText::FSText(FSText&& mv)
    : fx(mv.fx)
    , Line(mv.Line)
    , type(mv.type)
{
    mv.fx = nullptr;
    mv.Line = nullptr;
}

FSWriter::WriterImpl::FSText::~FSText() {
    if (fx) {
        delete fx;
        fx=NULL;
    }

    if (type == TT_BOX) {
        if (Box) {
            delete Box;
            Box = nullptr;
        }
    } else if (type == TT_LINE) {
        if (Line) {
            delete Line;
            Line = nullptr;
        }
    }

}

TypeText FSWriter::WriterImpl::FSText::Type() const {
    return type;
}
