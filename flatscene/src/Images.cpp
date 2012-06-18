#include "Library.h"
#include "ImagesImpl.h"
#include "ScreenImpl.h"

namespace FlatScene {

Images::Images() : _impl(new ImagesImpl) {}

Images::~Images() {
    clear();
    delete _impl;
}

void Images::clear() {
    SpritesetCollection::iterator iter ;
    Spriteset* pspt ;
    while ( !(*_impl).set.empty ( ) )
    {
        iter = (*_impl).set.begin ( ) ;
        pspt = iter->second ;
        (*_impl).set.erase ( iter ) ;
        if (SDL_GetVideoSurface())
            Screen::I()._impl->spritesetToDelete.push_back(pspt); // delete sptset;
        else
            delete pspt;
    }
    (*_impl).count.clear();
    while (!(*_impl).lastIndexAdded.empty()) {
        (*_impl).lastIndexAdded.pop();
    }
}

int Images::add(const char* name,Uint8 mode) {
    Spriteset* sptset;

    int ret=search(name);
    if (ret <0) {
        if (!(*_impl).lastIndexAdded.empty()) {
            ret=(*_impl).lastIndexAdded.top();
            (*_impl).lastIndexAdded.pop();
        } else
            for (int i=0 ; ret < 0 ; i++)
                if ((*_impl).set.find(i)==(*_impl).set.end())
                    ret = i;

        sptset=new Spriteset(name,mode);
        (*_impl).set[ret]=sptset;
    } else {
        sptset=(*_impl).set[ret];
    }
    ++(*_impl).count[sptset];
    return ret;
}

int Images::remove(Uint32 n) {
    if ((*_impl).set.find(n)!=(*_impl).set.end()) {
        Spriteset* sptset = (*_impl).set[n];
        int c=--(*_impl).count[sptset];
        if (c < 1)  {
            if (c==0) {
                if (SDL_GetVideoSurface())
                    Screen::I()._impl->spritesetToDelete.push_back(sptset); // delete sptset;
                else
                    delete sptset;
                (*_impl).set.erase((*_impl).set.find(n));
                (*_impl).count.erase((*_impl).count.find(sptset));
                (*_impl).lastIndexAdded.push(n);
            } else {
                Library::I().Error("Cantidad de Spriteset violada.",TE_controlViolation);
                return FRACASO;
            }
        }

        return EXITO;
    }

    Library::I().Error("No existe el Spriteset que se pretende eliminar.",TE_controlViolation);
    return FRACASO;

}

int Images::search(const char* name) {
    for (SpritesetCollection::iterator it = (*_impl).set.begin();it!=(*_impl).set.end();++it) {
        if (strcmp(it->second->getName().c_str(),name)==0) {
            return it->first;
        }
    }
    return FRACASO;
}

int Images::search(Spriteset* object) {
    for (SpritesetCollection::iterator it = (*_impl).set.begin();it!=(*_impl).set.end();++it) {
        if (it->second==object) {
            return it->first;
        }
    }
    return FRACASO;
}

Spriteset* Images::get(Uint32 n) {
    if ((*_impl).set.find(n)!=(*_impl).set.end())
        return (*_impl).set[n];
    else
        return NULL;
}

int Images::size() {
    return ((*_impl).set.size());
}

int Images::getCount(Uint32 n) {
    Uint32 ret = 0;
    if ((*_impl).count.find(get(n))!=(*_impl).count.end())
        ret = (*_impl).count[get(n)];
    return ret;
}
#ifdef GLOBAL_SINGLETON_REFERENCES
Images& Img = Images::I();
#endif

} // flatscene
