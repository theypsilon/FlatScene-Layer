#include "FSWriterImpl.h"

namespace flatscene {

Writer::WriterImpl::FSText::FSText(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next) 
    : fx(nullptr)
    , Object(new FSTextBox(file,text,x,y,Lim,ttf_fnt,next))
    , type(TT_BOX)
{}

Writer::WriterImpl::FSText::FSText() 
    : fx(nullptr)
    , Object(new SLineText)
    , type(TT_LINE)
{}

Writer::WriterImpl::FSText::FSText(FSText&& mv)
    : fx(std::move(mv.fx))
    , Object(std::move(mv.Object))
    , type(mv.type)
{}

Writer::WriterImpl::FSText::~FSText() {}

TypeText Writer::WriterImpl::FSText::Type() const {
    return type;
}

} // flatscene
