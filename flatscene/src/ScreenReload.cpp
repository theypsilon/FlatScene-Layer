#include "Library.h"
#include "ImagesImpl.h"
#include "ScreenImpl.h"

namespace FlatScene {

void Screen::ScreenImpl::deleteResources() {
    
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

void Screen::ScreenImpl::saveResources(GraphicResources &info) {

    Images& img = Images::I();
    
    for (SpritesetCollection::iterator it=img._impl->set.begin(),kt=img._impl->set.end();it!=kt;++it)   {
        SpritesetInfo aux;
        aux.number = it->first;
        aux.name =  it->second->getName();
        aux.mode = it->second->getMode();
        aux.times = img.getCount(it->first);
        info.push_back(aux);
    }

    img.clear();

    deleteResources();

}

void Screen::ScreenImpl::reloadResources(GraphicResources &info) {

    Images& img = Images::I();

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

} // flatscene