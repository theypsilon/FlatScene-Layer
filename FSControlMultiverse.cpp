#include "FSControlMultiverse.h"
#include "FSLibrary.h"

CControlMultiverse* CControlMultiverse::singleton=NULL;

CControlMultiverse::CControlMultiverse() : admin(NULL), working(false) {
	singleton = NULL;

	if (singleton) {
		CLibrary::Error("CMultiverse ya estaba creado.",TE_controlViolation);
		return;
	}

	singleton = this;
}

CControlMultiverse::~CControlMultiverse() {
	for (MultiverseByAdmin::iterator it = session.begin(),jt=session.end();it!=jt;++it) {
		setAdmin(it->first);
		clear();
	}
}

CMessageHandler* CControlMultiverse::setAdmin(CMessageHandler* newAdmin) {
	if (newAdmin != admin) {
		if (newAdmin) {
			unisCurrent = session[newAdmin];
			if (!unisCurrent) {
				unisCurrent = new UniverseCollection();
				session[newAdmin]=unisCurrent;
			}
		}

		CMessageHandler* oldAdmin = admin;
		admin = newAdmin;
		return oldAdmin;
	} else {
		return admin;
	}
}

CUniverse* CControlMultiverse::add(CUniverse* uni,Uint8 slot) {

	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (!admin || !uni || slot == 255) {
		if (!admin)	CLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		else if (!uni) CLibrary::Error("No se puede realizar dicha operación si no se indica un nombre correcto.");
		else if (slot == 255) CLibrary::Error("Slot limit exceded. 255 no es posible.");
		else CLibrary::Error("Error desconocido.");

		if (uni) {
			working = true;
			delete uni;
			working = false;
		}

		return NULL;
	}

	string& stName = uni->getName();
	CUniverse* uniDev = universeNamed(stName,slot);
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

CUniverse* CControlMultiverse::universeNamed(const char* uniName,Uint8 slot) {
	if (uniName)	{
		std::string cad(uniName);
		return universeNamed(cad,slot);
	} else {
		CLibrary::Error("No se ha indicado un nombre para el universo.");
		return NULL;
	}
}


CUniverse* CControlMultiverse::universeNamed(std::string uniName,Uint8 slot) {
	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (admin) {
		CUniverse* uniDev=NULL;
		for (UniverseCollection::iterator it=unisCurrent->begin(),et=unisCurrent->end();it!=et;++it)  {
			if ((*it)->getName()==uniName && (*it)->slot == slot) {
				uniDev = (*it);
				break;
			}
		}
		return uniDev;
	} else {
		CLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		return NULL;
	}
}

int CControlMultiverse::size() {
	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (admin)
		return unisCurrent->size();
	else {
		CLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		return 0;
	}
}

void CControlMultiverse::erase(CUniverse *mapKilled) {
	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (!admin || !mapKilled)
		if (!admin) CLibrary::Error("No se puede borrar el universo si el administrador no existe.");
		else if (!mapKilled) CLibrary::Error("No se puede borrar el universo porque no es válido.");
		else CLibrary::Error("Error desconocido.");

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

void CControlMultiverse::clear() {
	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (admin) {
		if (unisCurrent)	{
			UniverseCollection::iterator it ;
			while ( !unisCurrent->empty ( ) )
			{
				it = unisCurrent->begin ( ) ;
				CUniverse* m = *it ;
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

UniverseCollection::iterator CControlMultiverse::begin() {
	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());

	if (admin)
		return unisCurrent->begin();
	else {
		CLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		if (session.find(NULL)==session.end())
			session[NULL]= new UniverseCollection();
		return session[NULL]->begin();
	}
}

UniverseCollection::iterator CControlMultiverse::end() {
	if (admin != CLibrary::getActualEngine())
		setAdmin(CLibrary::getActualEngine());
	
	if (admin)
		return unisCurrent->end();
	else {
		CLibrary::Error("No se puede realizar dicha operación si el administrador no existe.");
		if (session.find(NULL)==session.end())
			session[NULL]= new UniverseCollection();
		return session[NULL]->begin();
	}
}

CControlMultiverse CMultiverse;
