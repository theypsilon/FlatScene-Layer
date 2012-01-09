#include "FSScreen.h"
#include "FSLibrary.h"
#include "FSWriterImpl.h"

void FSScreen::saveResources(GraphicResources &info) {
	
	for (SpritesetCollection::iterator it=CImg.set.begin(),kt=CImg.set.end();it!=kt;++it)	{
		SpritesetInfo aux;
		aux.number = it->first;
		aux.name =	it->second->getName();
		aux.mode = it->second->getMode();
		aux.times = CImg.getCount(it->first);
		info.push_back(aux);
	}

	CImg.clear();

	for (map<int,FSWriter::WriterImpl::SFont*>::iterator it=Write._impl->Fonts.begin();it!=Write._impl->Fonts.end();++it) {
		map<Uint16,FSCanvas*>& chars = it->second->render;
		for (map<Uint16,FSCanvas*>::iterator jt=chars.begin();jt!=chars.end();++jt) {
			imageToDelete.push_back(jt->second); // delete
			jt->second = NULL;
		}
	}

	for (map<FSEngine*,FSWriter::WriterImpl::SData*>::iterator it=Write._impl->session.begin();it!=Write._impl->session.end();++it) {
		map<int,FSWriter::WriterImpl::FSText*>& auxBoxs = it->second->Texts;
		for (map<int,FSWriter::WriterImpl::FSText*>::iterator jt=auxBoxs.begin();jt!=auxBoxs.end();++jt) {
			if (jt->second->Type() == TT_BOX && jt->second->Box)
				jt->second->Box->deleteBox();
		}
	}

	deleteResources();

}

void FSScreen::reloadResources(GraphicResources &info) {
	
	for (map<int,FSWriter::WriterImpl::SFont*>::iterator it=Write._impl->Fonts.begin();it!=Write._impl->Fonts.end();++it) {
		map<Uint16,FSCanvas*>& chars = it->second->render;
		for (map<Uint16,FSCanvas*>::iterator jt=chars.begin();jt!=chars.end();++jt) {
			if (!jt->second) {
				jt->second = new FSSprite(FSCanvas::toSCanvas(TTF_RenderGlyph_Blended(it->second->fuente,jt->first,Write._impl->data->fgcolor)));
			} else {
				return FSLibrary::Error("No se puede recargar el recurso glyph porque no había sido descargado anteriormente.");
			}
		}
	}

	for (map<FSEngine*,FSWriter::WriterImpl::SData*>::iterator it=Write._impl->session.begin();it!=Write._impl->session.end();++it) {
		map<int,FSWriter::WriterImpl::FSText*>& auxBoxs = it->second->Texts;
		for (map<int,FSWriter::WriterImpl::FSText*>::iterator jt=auxBoxs.begin();jt!=auxBoxs.end();++jt) {
			if (jt->second->Type() == TT_BOX && jt->second->Box)
				jt->second->Box->createBox();
		}
	}

	int number = 0;
	int aux = -1;

	for (GraphicResources::iterator it=info.begin(),et=info.end();it!=et;++it) {
		number = CImg.add((*it).name.c_str(),(*it).mode);
		if (number != it->number) {
			CImg.set[it->number] = CImg.set[number];
			CImg.set.erase(number);
			CImg.lastIndexAdded.push(number);
		}
		CImg.count[CImg.get(it->number)]=it->times;
	}
}
