#include "FSMultiverseImpl.h"
#include "FSLibrary.h"

FSMultiverse* FSMultiverse::MultiverseImpl::singleton=NULL;
FSMessageHandler* FSMultiverse::MultiverseImpl::setAdmin(FSMessageHandler* newAdmin) {
    if (newAdmin != admin) {
        if (newAdmin) {
            unisCurrent = session[newAdmin];
            if (!unisCurrent) {
                unisCurrent = new UniverseCollection();
                session[newAdmin]=unisCurrent;
            }
        }

        FSMessageHandler* oldAdmin = admin;
        admin = newAdmin;
        return oldAdmin;
    } else {
        return admin;
    }
}

FSMultiverse& FSMultiverse::Instance() {
    if (!MultiverseImpl::singleton)
        MultiverseImpl::singleton = new FSMultiverse;

    return *MultiverseImpl::singleton;
}


FSMultiverse::FSMultiverse() : _impl(new MultiverseImpl) {

    _impl->admin = NULL;
    _impl->unisCurrent = NULL;
    _impl->working = false;

    if (MultiverseImpl::singleton) {
        FSLibrary::Error("Cosmos ya estaba creado.",TE_controlViolation);
    }
}

FSMultiverse::~FSMultiverse() {
    for (MultiverseByAdmin::iterator it = (*_impl).session.begin(),jt=(*_impl).session.end();it!=jt;++it) {
        _impl->setAdmin(it->first);
        clear();
    }

    delete _impl;
}

FSUniverse* FSMultiverse::add(FSUniverse* uni,Uint8 slot) {

    if ((*_impl).admin != FSLibrary::getActualEngine())
        _impl->setAdmin(FSLibrary::getActualEngine());

    if (!(*_impl).admin || !uni || slot == 255) {
        if (!(*_impl).admin)    FSLibrary::Error("No se puede realizar dicha operaci�n si el (*_impl).administrador no existe.");
        else if (!uni) FSLibrary::Error("No se puede realizar dicha operaci�n si no se indica un nombre correcto.");
        else if (slot == 255) FSLibrary::Error("Slot limit exceded. 255 no es posible.");
        else FSLibrary::Error("Error desconocido.");

        if (uni) {
            (*_impl).working = true;
            delete uni;
            (*_impl).working = false;
        }

        return NULL;
    }

    string& stName = uni->getName();
    FSUniverse* uniDev = universeNamed(stName,slot);
    if (uniDev==NULL) {
        uniDev = uni;
        uni->slot = slot;
        uni->setParent((*_impl).admin);
        (*_impl).unisCurrent->push_back(uni);
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
        FSLibrary::Error("No se ha indicado un nombre para el universo.");
        return NULL;
    }
}


FSUniverse* FSMultiverse::universeNamed(std::string uniName,Uint8 slot) {
    if ((*_impl).admin != FSLibrary::getActualEngine())
        _impl->setAdmin(FSLibrary::getActualEngine());

    if ((*_impl).admin) {
        FSUniverse* uniDev=NULL;
        for (UniverseCollection::iterator it=(*_impl).unisCurrent->begin(),et=(*_impl).unisCurrent->end();it!=et;++it)  {
            if ((*it)->getName()==uniName && (*it)->slot == slot) {
                uniDev = (*it);
                break;
            }
        }
        return uniDev;
    } else {
        FSLibrary::Error("No se puede realizar dicha operaci�n si el (*_impl).administrador no existe.");
        return NULL;
    }
}

int FSMultiverse::size() {
    if ((*_impl).admin != FSLibrary::getActualEngine())
        _impl->setAdmin(FSLibrary::getActualEngine());

    if ((*_impl).admin)
        return (*_impl).unisCurrent->size();
    else {
        FSLibrary::Error("No se puede realizar dicha operaci�n si el (*_impl).administrador no existe.");
        return 0;
    }
}

void FSMultiverse::erase(FSUniverse *mapKilled) {
    if ((*_impl).admin != FSLibrary::getActualEngine())
        _impl->setAdmin(FSLibrary::getActualEngine());

    if (!(*_impl).admin || !mapKilled)
        if (!(*_impl).admin) FSLibrary::Error("No se puede borrar el universo si el (*_impl).administrador no existe.");
        else if (!mapKilled) FSLibrary::Error("No se puede borrar el universo porque no es v�lido.");
        else FSLibrary::Error("Error desconocido.");

    bool enc = false;

    for (UniverseCollection::iterator it=(*_impl).unisCurrent->begin(),et=(*_impl).unisCurrent->end();it!=et;++it) {
        if (mapKilled==*it) {
            (*_impl).unisCurrent->erase(it);
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
    if ((*_impl).admin != FSLibrary::getActualEngine())
        _impl->setAdmin(FSLibrary::getActualEngine());

    if ((*_impl).admin) {
        if ((*_impl).unisCurrent)   {
            UniverseCollection::iterator it ;
            while ( !(*_impl).unisCurrent->empty ( ) )
            {
                it = (*_impl).unisCurrent->begin ( ) ;
                FSUniverse* m = *it ;
                (*_impl).unisCurrent->erase ( it ) ;
                (*_impl).working=true;
                delete m ;
                (*_impl).working=false;
            }
            delete (*_impl).unisCurrent;
            (*_impl).unisCurrent=NULL;

            (*_impl).session.erase((*_impl).session.find((*_impl).admin));

            (*_impl).admin = NULL;
        }
    }
}

UniverseCollection::iterator FSMultiverse::begin() {
    if ((*_impl).admin != FSLibrary::getActualEngine())
        _impl->setAdmin(FSLibrary::getActualEngine());

    if ((*_impl).admin)
        return (*_impl).unisCurrent->begin();
    else {
        FSLibrary::Error("No se puede realizar dicha operaci�n si el (*_impl).administrador no existe.");
        if ((*_impl).session.find(NULL)==(*_impl).session.end())
            (*_impl).session[NULL]= new UniverseCollection();
        return (*_impl).session[NULL]->begin();
    }
}

UniverseCollection::iterator FSMultiverse::end() {
    if ((*_impl).admin != FSLibrary::getActualEngine())
        _impl->setAdmin(FSLibrary::getActualEngine());

    if ((*_impl).admin)
        return (*_impl).unisCurrent->end();
    else {
        FSLibrary::Error("No se puede realizar dicha operaci�n si el (*_impl).administrador no existe.");
        if ((*_impl).session.find(NULL)==(*_impl).session.end())
            (*_impl).session[NULL]= new UniverseCollection();
        return (*_impl).session[NULL]->begin();
    }
}

FSMultiverse& Cosmos = FSMultiverse::Instance();
