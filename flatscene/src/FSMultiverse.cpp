#include "FSMultiverseImpl.h"
#include "FSLibrary.h"

namespace flatscene {

Multiverse::Multiverse() : _impl(new MultiverseImpl) {
    _impl->working = false;
}

Multiverse::~Multiverse() {
    clear();

    delete _impl;
}

Universe* Multiverse::add(Universe* uni,Uint8 slot) {

    if (!uni || slot == 255) {
        if (!uni) Library::I().Error("No se puede realizar dicha operaci�n si no se indica un nombre correcto.");
        else if (slot == 255) Library::I().Error("Slot limit exceded. 255 no es posible.");
        else Library::I().Error("Error desconocido.");

        if (uni) {
            (*_impl).working = true;
            delete uni;
            (*_impl).working = false;
        }

        return NULL;
    }

    std::string& stName = uni->getName();
    Universe* uniDev = universeNamed(stName,slot);
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

Universe* Multiverse::universeNamed(const char* uniName,Uint8 slot) {
    if (uniName)    {
        std::string cad(uniName);
        return universeNamed(cad,slot);
    } else {
        Library::I().Error("No se ha indicado un nombre para el universo.");
        return NULL;
    }
}


Universe* Multiverse::universeNamed(std::string uniName,Uint8 slot) {
    Universe* uniDev=NULL;
    for (auto it=(*_impl).unis.begin(),et=(*_impl).unis.end();it!=et;++it)  {
        if ((*it)->getName()==uniName && (*it)->slot == slot) {
            uniDev = (*it);
            break;
        }
    }
    return uniDev;
}

int Multiverse::size() {
    return (*_impl).unis.size();
}

void Multiverse::erase(Universe *mapKilled) {

    if (!mapKilled) {
        Library::I().Error("No se puede borrar el universo porque no es v�lido.");
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

void Multiverse::clear() {
    UniverseCollection::iterator it ;
    while ( !(*_impl).unis.empty() ) {
        it = (*_impl).unis.begin();
        Universe* m = *it;
        (*_impl).unis.erase( it );
        (*_impl).working=true;
        delete m;
        (*_impl).working=false;
    }
}

UniverseCollection::iterator Multiverse::begin() {
    return (*_impl).unis.begin();
}

UniverseCollection::iterator Multiverse::end() {
    return (*_impl).unis.end();
}

} // flatscene
