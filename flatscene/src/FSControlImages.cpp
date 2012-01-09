#include "FSControlImages.h"
#include "FSLibrary.h"

FSControlImages* FSControlImages::singleton=NULL;

map<TypeResource,void (*)(void*)> FSCanvas::procRenders;

FSControlImages::FSControlImages() {

	FSCanvas::initProcRenders();

	singleton = NULL;

	if (singleton) {
		FSLibrary::Error("CControlImages (CImg) ya estaba creado.",TE_controlViolation);
		return;
	}

	singleton = this;

}

FSControlImages::~FSControlImages() {
	clear();
}

void FSControlImages::clear() {
	SpritesetCollection::iterator iter ;
	FSSpriteset* pspt ;
	while ( !set.empty ( ) )
	{
		iter = set.begin ( ) ;
		pspt = iter->second ;
		set.erase ( iter ) ;
		if (SDL_GetVideoSurface())
			FSScreen::spritesetToDelete.push_back(pspt); // delete sptset;
		else
			delete pspt;
	}
	count.clear();
	while (!lastIndexAdded.empty()) {
		lastIndexAdded.pop();
	}
}

int FSControlImages::add(const char* name,Uint8 mode) {
	FSSpriteset* sptset;

	int ret=search(name);
	if (ret <0) {
		if (!lastIndexAdded.empty()) {
			ret=lastIndexAdded.top();
			lastIndexAdded.pop();
		} else
			for (int i=0 ; ret < 0 ; i++)
				if (set.find(i)==set.end())
					ret = i;

		sptset=new FSSpriteset(name,mode);
		set[ret]=sptset;
	} else {
		sptset=set[ret];
	}
	++count[sptset];
	return ret;
}

int FSControlImages::remove(Uint32 n) {
	if (set.find(n)!=set.end()) {
		FSSpriteset* sptset = set[n];
		int c=--count[sptset];
		if (c < 1)  {
			if (c==0) {
				if (SDL_GetVideoSurface())
					FSScreen::spritesetToDelete.push_back(sptset); // delete sptset;
				else
					delete sptset;
				set.erase(set.find(n));
				count.erase(count.find(sptset));
				lastIndexAdded.push(n);
			} else {
				FSLibrary::Error("Cantidad de Spriteset violada.",TE_controlViolation);
				return FRACASO;
			}
		}

		return EXITO;
	}

	FSLibrary::Error("No existe el Spriteset que se pretende eliminar.",TE_controlViolation);
	return FRACASO;

}

int FSControlImages::search(const char* name) {
	for (SpritesetCollection::iterator it = set.begin();it!=set.end();++it) {
		if (strcmp(it->second->getName().c_str(),name)==0) {
			return it->first;
		}
	}
	return FRACASO;
}

int FSControlImages::search(FSSpriteset* object) {
	for (SpritesetCollection::iterator it = set.begin();it!=set.end();++it) {
		if (it->second==object) {
			return it->first;
		}
	}
	return FRACASO;
}

FSSpriteset* FSControlImages::get(Uint32 n) {
	if (set.find(n)!=set.end())
		return set[n];
	else
		return NULL;
}

int FSControlImages::size() {
	return (set.size());
}

int FSControlImages::getCount(Uint32 n) {
	Uint32 ret = 0;
	if (count.find(get(n))!=count.end())
		ret = count[get(n)];
	return ret;
}

FSControlImages Img;
