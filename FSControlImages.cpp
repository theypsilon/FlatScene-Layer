#include "ControlImages.h"
#include "Library.h"

CControlImages* CControlImages::singleton=NULL;

map<TypeResource,void (*)(void*)> CImage::procRenders;

CControlImages::CControlImages() {

	CImage::initProcRenders();

	singleton = NULL;

	if (singleton) {
		CLibrary::Error("CControlImages (CImg) ya estaba creado.",TE_controlViolation);
		return;
	}

	singleton = this;

}

CControlImages::~CControlImages() {
	clear();
}

void CControlImages::clear() {
	SpritesetCollection::iterator iter ;
	CSpriteset* pspt ;
	while ( !set.empty ( ) )
	{
		iter = set.begin ( ) ;
		pspt = iter->second ;
		set.erase ( iter ) ;
		CScreen::spritesetToDelete.push_back(pspt); // delete sptset;
	}
	count.clear();
	while (!lastIndexAdded.empty()) {
		lastIndexAdded.pop();
	}
}

int CControlImages::add(const char* name,Uint8 mode) {
	CSpriteset* sptset;

	int ret=search(name);
	if (ret <0) {
		if (!lastIndexAdded.empty()) {
			ret=lastIndexAdded.top();
			lastIndexAdded.pop();
		} else
			for (int i=0 ; ret < 0 ; i++)
				if (set.find(i)==set.end())
					ret = i;

		sptset=new CSpriteset(name,mode);
		set[ret]=sptset;
	} else {
		sptset=set[ret];
	}
	++count[sptset];
	return ret;
}

int CControlImages::remove(Uint32 n) {
	if (set.find(n)!=set.end()) {
		CSpriteset* sptset = set[n];
		int c=--count[sptset];
		if (c < 1)  {
			if (c==0) {
				CScreen::spritesetToDelete.push_back(sptset); // delete sptset;
				set.erase(set.find(n));
				count.erase(count.find(sptset));
				lastIndexAdded.push(n);
			} else {
				CLibrary::Error("Cantidad de Spriteset violada.",TE_controlViolation);
				return FRACASO;
			}
		}

		return EXITO;
	}

	CLibrary::Error("No existe el Spriteset que se pretende eliminar.",TE_controlViolation);
	return FRACASO;

}

int CControlImages::search(const char* name) {
	for (SpritesetCollection::iterator it = set.begin();it!=set.end();++it) {
		if (strcmp(it->second->getName().c_str(),name)==0) {
			return it->first;
		}
	}
	return FRACASO;
}

int CControlImages::search(CSpriteset* object) {
	for (SpritesetCollection::iterator it = set.begin();it!=set.end();++it) {
		if (it->second==object) {
			return it->first;
		}
	}
	return FRACASO;
}

CSpriteset* CControlImages::get(Uint32 n) {
	if (set.find(n)!=set.end())
		return set[n];
	else
		return NULL;
}

int CControlImages::size() {
	return (set.size());
}

int CControlImages::getCount(Uint32 n) {
	Uint32 ret = 0;
	if (count.find(get(n))!=count.end())
		ret = count[get(n)];
	return ret;
}

CControlImages CImg;