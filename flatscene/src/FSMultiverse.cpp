#include "FSMultiverseImpl.h"
#include "FSLibrary.h"

namespace flatscene {

FSMultiverse::FSMultiverse() : _impl(new MultiverseImpl) {
    _impl->working = false;
}

FSMultiverse::~FSMultiverse() {
    clear();

    delete _impl;
}

FSUniverse* FSMultiverse::add(FSUniverse* uni,Uint8 slot) {

    if (!uni || slot == 255) {
        if (!uni) FSLibrary::I().Error("No se puede realizar dicha operaci�n si no se indica un nombre correcto.");
        else if (slot == 255) FSLibrary::I().Error("Slot limit exceded. 255 no es posible.");
        else FSLibrary::I().Error("Error desconocido.");

        if (uni) {
            (*_impl).working = true;
            delete uni;
            (*_impl).working = false;
        }

        return NULL;
    }

    std::string& stName = uni->getName();
    FSUniverse* uniDev = universeNamed(stName,slot);
    if (uniDev==nullptr) {
        uniDev = uni;
        uni->slot = slot;
        (*_impl).unis.push_back(uni);
    } else {
        (*_impl).working = true;
        delete uni;
        (*_impl).working = false;
    }

    return uniDev;
}

FSUniverse* FSMultiverse::universeNamed(const char* uniName,Uint8 slot) {
    if (uniName)    {
        std::string cad(uniName);
        return universeNamed(cad,slot);
    } else {
        FSLibrary::I().Error("No se ha indicado un nombre para el universo.");
        return NULL;
    }
}


FSUniverse* FSMultiverse::universeNamed(std::string uniName,Uint8 slot) {
    FSUniverse* uniDev=NULL;
    for (auto it=(*_impl).unis.begin(),et=(*_impl).unis.end();it!=et;++it)  {
        if ((*it)->getName()==uniName && (*it)->slot == slot) {
            uniDev = (*it);
            break;
        }
    }
    return uniDev;
}

int FSMultiverse::size() {
    return (*_impl).unis.size();
}

void FSMultiverse::erase(FSUniverse *mapKilled) {

    if (!mapKilled) {
        FSLibrary::I().Error("No se puede borrar el universo porque no es v�lido.");
        return;
    }

    bool enc = false;

    for (auto it=(*_impl).unis.begin(),et=(*_impl).unis.end();it!=et;++it) {
        if (mapKilled==*it) {
            (*_impl).unis.erase(it);
            enc=true;
            break;
        }
    }

    if (enc) {
        (*_impl).working=true;
        delete mapKilled;
        (*_impl).working=false;
    }

}

void FSMultiverse::clear() {
    UniverseCollection::iterator it ;
    while ( !(*_impl).unis.empty() ) {
        it = (*_impl).unis.begin();
        FSUniverse* m = *it;
        (*_impl).unis.erase( it );
        (*_impl).working=true;
        delete m;
        (*_impl).working=false;
    }
}

UniverseCollection::iterator FSMultiverse::begin() {
    return (*_impl).unis.begin();
}

UniverseCollection::iterator FSMultiverse::end() {
    return (*_impl).unis.end();
}

} // flatscene
