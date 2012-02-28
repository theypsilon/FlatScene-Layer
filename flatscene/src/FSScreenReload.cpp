#include "FSLibrary.h"
#include "FSWriterImpl.h"
#include "FSImagesImpl.h"
#include "FSScreenImpl.h"

void FSScreen::ScreenImpl::deleteResources() {
    
    for (auto it = spritesetToDelete.begin(), jt = spritesetToDelete.end() ; it!=jt;++it)
        delete (*it);

    spritesetToDelete.clear();
    
    for (auto it = spriteToDelete.begin(), jt = spriteToDelete.end() ; it!=jt;++it)
        delete (*it);

    spriteToDelete.clear();

    for (auto it = imageToDelete.begin(), jt = imageToDelete.end() ; it!=jt;++it)
        delete (*it);

    imageToDelete.clear();
}

void FSScreen::ScreenImpl::saveResources(GraphicResources &info) {

    FSImages& img = FSImages::I();
    FSWriter& writer = FSWriter::I();
    
    for (SpritesetCollection::iterator it=img._impl->set.begin(),kt=img._impl->set.end();it!=kt;++it)   {
        SpritesetInfo aux;
        aux.number = it->first;
        aux.name =  it->second->getName();
        aux.mode = it->second->getMode();
        aux.times = img.getCount(it->first);
        info.push_back(aux);
    }

    img.clear();

    for (auto it=writer._impl->Fonts.begin();it!=writer._impl->Fonts.end();++it) {
        std::map<Uint16,FSCanvas*>& chars = it->second->render;
        for (std::map<Uint16,FSCanvas*>::iterator jt=chars.begin();jt!=chars.end();++jt) {
            imageToDelete.push_back(jt->second); // delete
            jt->second = NULL;
        }
    }

    for (auto it=writer._impl->session.begin();it!=writer._impl->session.end();++it) {
        std::map<int,FSWriter::WriterImpl::FSText*>& auxBoxs = it->second->Texts;
        for (auto jt=auxBoxs.begin();jt!=auxBoxs.end();++jt) {
            if (jt->second->Type() == TT_BOX && jt->second->Box)
                jt->second->Box->deleteBox();
        }
    }

    deleteResources();

}

void FSScreen::ScreenImpl::reloadResources(GraphicResources &info) {

    FSImages& img = FSImages::I();
    FSWriter& writer = FSWriter::I();
    
    for (auto it=writer._impl->Fonts.begin();it!=writer._impl->Fonts.end();++it) {
        std::map<Uint16,FSCanvas*>& chars = it->second->render;
        for (auto jt=chars.begin();jt!=chars.end();++jt) {
            if (!jt->second) {
                jt->second = new FSSprite(FSCanvas::toSCanvas(TTF_RenderGlyph_Blended(it->second->fuente,jt->first,writer._impl->data->fgcolor)));
            } else {
                return FSLibrary::I().Error("No se puede recargar el recurso glyph porque no había sido descargado anteriormente.");
            }
        }
    }

    for (auto it=writer._impl->session.begin();it!=writer._impl->session.end();++it) {
        std::map<int,FSWriter::WriterImpl::FSText*>& auxBoxs = it->second->Texts;
        for (std::map<int,FSWriter::WriterImpl::FSText*>::iterator jt=auxBoxs.begin();jt!=auxBoxs.end();++jt) {
            if (jt->second->Type() == TT_BOX && jt->second->Box)
                jt->second->Box->createBox();
        }
    }

    int number = 0;
    int aux = -1;

    for (GraphicResources::iterator it=info.begin(),et=info.end();it!=et;++it) {
        number = img.add((*it).name.c_str(),(*it).mode);
        if (number != it->number) {
            img._impl->set[it->number] = img._impl->set[number];
            img._impl->set.erase(number);
            img._impl->lastIndexAdded.push(number);
        }
        img._impl->count[img.get(it->number)]=it->times;
    }
}
