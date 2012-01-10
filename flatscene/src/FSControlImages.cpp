#include "FSImagesImpl.h"
#include "FSLibrary.h"

FSControlImages* FSControlImages::ImagesImpl::singleton=NULL;

map<TypeResource,void (*)(void*)> FSCanvas::procRenders;

FSControlImages::FSControlImages() : _impl(new ImagesImpl) {

	FSCanvas::initProcRenders();

	ImagesImpl::singleton = NULL;

	if (ImagesImpl::singleton) {
		FSLibrary::Error("CControlImages (CImg) ya estaba creado.",TE_controlViolation);
		return;
	}

	ImagesImpl::singleton = this;

}

FSControlImages::~FSControlImages() {
	clear();
    delete _impl;
}

void FSControlImages::clear() {
	SpritesetCollection::iterator iter ;
	FSSpriteset* pspt ;
	while ( !(*_impl).set.empty ( ) )
	{
		iter = (*_impl).set.begin ( ) ;
		pspt = iter->second ;
		(*_impl).set.erase ( iter ) ;
		if (SDL_GetVideoSurface())
			FSScreen::spritesetToDelete.push_back(pspt); // delete sptset;
		else
			delete pspt;
	}
	(*_impl).count.clear();
	while (!(*_impl).lastIndexAdded.empty()) {
		(*_impl).lastIndexAdded.pop();
	}
}

int FSControlImages::add(const char* name,Uint8 mode) {
	FSSpriteset* sptset;

	int ret=search(name);
	if (ret <0) {
		if (!(*_impl).lastIndexAdded.empty()) {
			ret=(*_impl).lastIndexAdded.top();
			(*_impl).lastIndexAdded.pop();
		} else
			for (int i=0 ; ret < 0 ; i++)
				if ((*_impl).set.find(i)==(*_impl).set.end())
					ret = i;

		sptset=new FSSpriteset(name,mode);
		(*_impl).set[ret]=sptset;
	} else {
		sptset=(*_impl).set[ret];
	}
	++(*_impl).count[sptset];
	return ret;
}

int FSControlImages::remove(Uint32 n) {
	if ((*_impl).set.find(n)!=(*_impl).set.end()) {
		FSSpriteset* sptset = (*_impl).set[n];
		int c=--(*_impl).count[sptset];
		if (c < 1)  {
			if (c==0) {
				if (SDL_GetVideoSurface())
					FSScreen::spritesetToDelete.push_back(sptset); // delete sptset;
				else
					delete sptset;
				(*_impl).set.erase((*_impl).set.find(n));
				(*_impl).count.erase((*_impl).count.find(sptset));
				(*_impl).lastIndexAdded.push(n);
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
	for (SpritesetCollection::iterator it = (*_impl).set.begin();it!=(*_impl).set.end();++it) {
		if (strcmp(it->second->getName().c_str(),name)==0) {
			return it->first;
		}
	}
	return FRACASO;
}

int FSControlImages::search(FSSpriteset* object) {
	for (SpritesetCollection::iterator it = (*_impl).set.begin();it!=(*_impl).set.end();++it) {
		if (it->second==object) {
			return it->first;
		}
	}
	return FRACASO;
}

FSSpriteset* FSControlImages::get(Uint32 n) {
	if ((*_impl).set.find(n)!=(*_impl).set.end())
		return (*_impl).set[n];
	else
		return NULL;
}

int FSControlImages::size() {
	return ((*_impl).set.size());
}

int FSControlImages::getCount(Uint32 n) {
	Uint32 ret = 0;
	if ((*_impl).count.find(get(n))!=(*_impl).count.end())
		ret = (*_impl).count[get(n)];
	return ret;
}

FSControlImages Img;
