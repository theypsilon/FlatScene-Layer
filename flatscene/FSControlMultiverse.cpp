#include "FSControlMultiverse.h"
#include "FSLibrary.h"

FSControlMultiverse* FSControlMultiverse::singleton=NULL;

FSControlMultiverse::FSControlMultiverse() : admin(NULL), working(false) {
	singleton = NULL;

	if (singleton) {
		FSLibrary::Error("CMultiverse ya estaba creado.",TE_controlViolation);
		return;
	}

	singleton = this;
}

FSControlMultiverse::~FSControlMultiverse() {
	for (MultiverseByAdmin::iterator it = session.begin(),jt=session.end();it!=jt;++it) {
		setAdmin(it->first);
		clear();
	}
}

FSMessageHandler* FSControlMultiverse::setAdmin(FSMessageHandler* newAdmin) {
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

FSUniverse* FSControlMultiverse::add(FSUniverse* uni,Uint8 slot) {

	if (admin != FSLibrary::getActualEngine())
		setAdmin(FSLibrary::getActualEngine());

	if (!admin || !uni || slot == 255) {
		if (!admin)	FSLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		else if (!uni) FSLibrary::Error("No se puede realizar dicha operación si no se indica un nombre correcto.");
		else if (slot == 255) FSLibrary::Error("Slot limit exceded. 255 no es posible.");
		else FSLibrary::Error("Error desconocido.");

		if (uni) {
			working = true;
			delete uni;
			working = false;
		}

		return NULL;
	}

	string& stName = uni->getName();
	FSUniverse* uniDev = universeNamed(stName,slot);
	if (uniDev==NULL) {
		uniDev = uni;
		uni->slot = slot;
		uni->setParent(admin);
		unisCurrent->push_back(uni);
	} else {
		working = true;
		delete uni;
		working = false;
	}

	return uniDev;
}

FSUniverse* FSControlMultiverse::universeNamed(const char* uniName,Uint8 slot) {
	if (uniName)	{
		std::string cad(uniName);
		return universeNamed(cad,slot);
	} else {
		FSLibrary::Error("No se ha indicado un nombre para el universo.");
		return NULL;
	}
}


FSUniverse* FSControlMultiverse::universeNamed(std::string uniName,Uint8 slot) {
	if (admin != FSLibrary::getActualEngine())
		setAdmin(FSLibrary::getActualEngine());

	if (admin) {
		FSUniverse* uniDev=NULL;
		for (UniverseCollection::iterator it=unisCurrent->begin(),et=unisCurrent->end();it!=et;++it)  {
			if ((*it)->getName()==uniName && (*it)->slot == slot) {
				uniDev = (*it);
				break;
			}
		}
		return uniDev;
	} else {
		FSLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		return NULL;
	}
}

int FSControlMultiverse::size() {
	if (admin != FSLibrary::getActualEngine())
		setAdmin(FSLibrary::getActualEngine());

	if (admin)
		return unisCurrent->size();
	else {
		FSLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		return 0;
	}
}

void FSControlMultiverse::erase(FSUniverse *mapKilled) {
	if (admin != FSLibrary::getActualEngine())
		setAdmin(FSLibrary::getActualEngine());

	if (!admin || !mapKilled)
		if (!admin) FSLibrary::Error("No se puede borrar el universo si el administrador no existe.");
		else if (!mapKilled) FSLibrary::Error("No se puede borrar el universo porque no es válido.");
		else FSLibrary::Error("Error desconocido.");

	bool enc = false;

	for (UniverseCollection::iterator it=unisCurrent->begin(),et=unisCurrent->end();it!=et;++it) {
		if (mapKilled==*it) {
			unisCurrent->erase(it);
			enc=true;
			break;
		}
	}

	if (enc) {
		working=true;
		delete mapKilled;
		working=false;
	}

}

void FSControlMultiverse::clear() {
	if (admin != FSLibrary::getActualEngine())
		setAdmin(FSLibrary::getActualEngine());

	if (admin) {
		if (unisCurrent)	{
			UniverseCollection::iterator it ;
			while ( !unisCurrent->empty ( ) )
			{
				it = unisCurrent->begin ( ) ;
				FSUniverse* m = *it ;
				unisCurrent->erase ( it ) ;
				working=true;
				delete m ;
				working=false;
			}
			delete unisCurrent;
			unisCurrent=NULL;

			session.erase(session.find(admin));

			admin = NULL;
		}	
	}
}

UniverseCollection::iterator FSControlMultiverse::begin() {
	if (admin != FSLibrary::getActualEngine())
		setAdmin(FSLibrary::getActualEngine());

	if (admin)
		return unisCurrent->begin();
	else {
		FSLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		if (session.find(NULL)==session.end())
			session[NULL]= new UniverseCollection();
		return session[NULL]->begin();
	}
}

UniverseCollection::iterator FSControlMultiverse::end() {
	if (admin != FSLibrary::getActualEngine())
		setAdmin(FSLibrary::getActualEngine());
	
	if (admin)
		return unisCurrent->end();
	else {
		FSLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		if (session.find(NULL)==session.end())
			session[NULL]= new UniverseCollection();
		return session[NULL]->begin();
	}
}

FSControlMultiverse CMultiverse;
