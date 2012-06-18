#include "FSWriterImpl.h"

namespace FlatScene {

Writer::WriterImpl::FSText::FSText(const char* file,const char* text,int x,int y,int Lim,SFont* ttf_fnt,int next) 
    : fx(nullptr)
    , Object(new FSTextBox(file,text,x,y,Lim,ttf_fnt,next))
{}

Writer::WriterImpl::FSText::FSText() 
    : fx(nullptr)
    , Object(new SLineText)
{}

Writer::WriterImpl::FSText::FSText(FSText&& mv)
    : fx(std::move(mv.fx))
    , Object(std::move(mv.Object))
{}

Writer::WriterImpl::FSText& Writer::WriterImpl::FSText::operator=(FSText&& mv) {
    fx = std::move(mv.fx);
    Object = std::move(mv.Object);
    return *this;
}

Writer::WriterImpl::FSText::~FSText() {}

} // flatscene
