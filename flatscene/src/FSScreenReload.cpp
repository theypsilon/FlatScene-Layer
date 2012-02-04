#include "FSScreen.h"
#include "FSLibrary.h"
#include "FSWriterImpl.h"
#include "FSImagesImpl.h"

void FSScreen::saveResources(GraphicResources &info) {
	
	for (SpritesetCollection::iterator it=Img._impl->set.begin(),kt=Img._impl->set.end();it!=kt;++it)	{
		SpritesetInfo aux;
		aux.number = it->first;
		aux.name =	it->second->getName();
		aux.mode = it->second->getMode();
		aux.times = Img.getCount(it->first);
		info.push_back(aux);
	}

	Img.clear();

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
				return FSLibrary::I().Error("No se puede recargar el recurso glyph porque no había sido descargado anteriormente.");
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
		number = Img.add((*it).name.c_str(),(*it).mode);
		if (number != it->number) {
			Img._impl->set[it->number] = Img._impl->set[number];
			Img._impl->set.erase(number);
			Img._impl->lastIndexAdded.push(number);
		}
		Img._impl->count[Img.get(it->number)]=it->times;
	}
}
