#include "Library.h"
#include "ImagesImpl.h"
#include "ScreenImpl.h"
#include "Algorithm.h"
#include <assert.h>

namespace FlatScene {

Images::Images() : _impl(new ImagesImpl) {}

Images::~Images() {
    clear();
    delete _impl;
}

void Images::clear() {
    Spriteset* pspt ;

    while ( !(*_impl).set.empty () ) {
        auto iter = begin((*_impl).set) ;
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
    if ((*_impl).set.find(n) == (*_impl).set.end())
        throw ControlViolationException("No existe el Spriteset que se pretende eliminar.");

    Spriteset* sptset = (*_impl).set[n];
    int c = --(*_impl).count[sptset];

    if (c < 1) {
        assert(c == 0); // "Cantidad de Spriteset violada."

        if (SDL_GetVideoSurface())
            Screen::I()._impl->spritesetToDelete.push_back(sptset); // delete sptset;
        else
            delete sptset;
        (*_impl).set.erase((*_impl).set.find(n));
        (*_impl).count.erase((*_impl).count.find(sptset));
        (*_impl).lastIndexAdded.push(n);
    }

    return EXITO;

}

int Images::search(const char* name) {
    for (const auto& pair : (*_impl).set)
        if (pair.second->getName() == name)
            return pair.first;

    return FRACASO;
}

int Images::search(Spriteset* object) {
    for (const auto& pair : (*_impl).set)
        if (pair.second == object)
            return pair.first;

    return FRACASO;
}

Spriteset* Images::get(Uint32 n) {
    return find_assoc((*_impl).set,n,nullptr);
}

int Images::size() {
    return (*_impl).set.size();
}

int Images::getCount(Uint32 n) {
    return find_assoc((*_impl).count,get(n),0);
}
#ifdef GLOBAL_SINGLETON_REFERENCES
Images& Img = Images::I();
#endif

} // flatscene
