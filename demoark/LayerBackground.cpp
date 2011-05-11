#include "LayerBackground.h"
#include "FSScreen.h"
#include "ScrollCamera.h"


//
//CLayerBG
//

CLayerBG::CLayerBG(CMap* mapa,CRectangle* area,CMessageHandler* pmhParent) : CMessageHandler(pmhParent) , area(area) ,  mapa(mapa)  {
}

CLayerBG::~CLayerBG(){
	/*
	if (clipping!=NULL)
		delete clipping;*/
}

void CLayerBG::setAlpha(Uint8 alpha){ this->alpha=alpha; }

Uint8 CLayerBG::getAlpha() { return alpha; }

void CLayerBG::setFlags(Uint8 flags) { this->flags=flags; }

Uint8 CLayerBG::getFlags() { return flags; }

void CLayerBG::setSize(Uint8 size) { this->size=size; }

Uint8 CLayerBG::getSize() { return size; }

void CLayerBG::clear() {
	//SDL_FillRect(background,area,negro);
}
void CLayerBG::init(float zoom) { }
void CLayerBG::refresh(int cx, int cy) {}

#ifdef MENSAJES_MSGIDS
int CLayerBG::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
	printf("Layer :: ");
	return CMessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
}
#endif

//
//CLayerDinamic
//

CLayerDinamic::CLayerDinamic(CMap* mapa,CRectangle* area,Uint8 num_tiles,CMessageHandler* pmhParent) : CLayerBG(mapa,area,pmhParent) {
	this->num_tiles=num_tiles;
	init();
}

CLayerDinamic::~CLayerDinamic() {
/*	for (int i=0;i<num_tiles;i++)
		delete (TileBG[i]);
	delete(TileBG);
	~CLayerBG();*/
}

void CLayerDinamic::init(float zoom) {
/*	TileBG**=TileBG*[num_tiles];
	
	for (int i=0;i<num_tiles;i++)
		TileBG[i]=new DinamicTile(i);

	refresh();*/
}

void CLayerDinamic::refresh() {
/*	if (clipping!=NULL) { ; }

	for (int i;i<num_tiles;i++)
		TileBG[i]->refresh(background);*/
}

CColor* CLayerDinamic::getPixel(Uint8 i,Uint8 w,Uint8 h) {
	return new CColor();
}

//
//CLayerUniform
//

CLayerUniform::CLayerUniform(CMap* mapa,CRectangle* area,TileBG** tile,CMessageHandler* pmhParent) : CLayerBG(mapa,area,pmhParent), tile(tile) {
	init();
}

CLayerUniform::~CLayerUniform() {
}



void CLayerUniform::init(float zoom) {
	float aux_w=floor(((float)area->getW())/((float)mapa->getTileW())/zoom + 0.9);
	float aux_h=floor(((float)area->getH())/((float)mapa->getTileH())/zoom + 0.9);
	tam_w=  ((int)aux_w)+1;
	tam_h= ((int)aux_h)+1;
}

void CLayerUniform::refresh(int cx, int cy) {
	CPoint p;

	for (int i=0;i<tam_w;i++) {
		for (int j=0;j<tam_h;j++) {
			TileBG ind =  tile[(j+cy)%mapa->getH()][(i+cx)%mapa->getW()];
			if (ind.graph>0) {
				ind.graph--;
				p.X()=i*mapa->getTileW();
				p.Y()=j*mapa->getTileH();

				if (ind.flags & 0x001) {
					p.X()+=mapa->getTileW();
				}

				if (ind.flags & 0x010) {
					p.Y()+=mapa->getTileH();
				}

				CImg.get(mapa->getTileset()+ind.fileGraph)->get(ind.graph)->put(p,ind.flags);
			} 
		}
	}
}

Uint8 CLayerUniform::getTam_H() { return tam_h; }

Uint8 CLayerUniform::getTam_W() { return tam_w; }

Uint8 CLayerUniform::getTile_H() { return tile_h; }

Uint8 CLayerUniform::getTile_W() { return tile_w; }

CColor* CLayerUniform::getPixel(Uint8 i, Uint8 j, Uint8 w, Uint8 h) {
	return new CColor();
}

//
//CLayerInf
//

CLayerInf::CLayerInf(CMap* mapa,CRectangle* area,TileBG** tile,bool pisable,CMessageHandler* pmhParent) : CLayerUniform(mapa,area,tile,pmhParent) , pisable(pisable) {
	init();
}

CLayerInf::CLayerInf(CMap* mapa,CRectangle* area,TileBG** tile,CMessageHandler* pmhParent) : CLayerUniform(mapa,area,tile,pmhParent) , pisable(true){
	init();
}

CLayerInf::~CLayerInf()  {
	//CLayerUniform::~CLayerUniform();
}

bool CLayerInf::isPisable() { return pisable; }
//
//CLayerSup
//

CLayerSup::CLayerSup(CMap* mapa,CRectangle* area,TileBG** tile,CMessageHandler* pmhParent) : CLayerUniform(mapa,area,tile,pmhParent) {
	init();
}
CLayerSup::~CLayerSup() {
	//CLayerUniform::~CLayerUniform();
}

//
//CLayerFondo
//
CLayerFondo::CLayerFondo(CMap* mapa,CRectangle* area,TileBG** tile,CMessageHandler* pmhParent) : CLayerUniform(mapa,area,tile,pmhParent) {
	init();
}
CLayerFondo::~CLayerFondo() {
	//CLayerUniform::~CLayerUniform();
}

CFloor::CFloor(CRectangle *area,CMap* mapa,int id, CMessageHandler* pmhParent) : 
CMessageHandler(pmhParent), area(area) , mapa(mapa), id(id) {

}

CFloor::~CFloor() {

	LayerBGCollection::iterator iter ;
	CLayerBG* pspt ;
	while ( !Layer.empty ( ) )
	{
		iter = Layer.begin ( ) ;
		pspt = *iter ;
		Layer.erase ( iter ) ;
		if (pspt!=NULL)
			delete pspt ;
	}

}

bool CFloor::capaSuperior(CLayerBG* layer) {
	return false;
}

void CFloor::refresh(int cx, int cy) {
	int capa=0;

	int tam_tile_w=mapa->getTileW();
	int tam_tile_h=mapa->getTileH();

	int cxl=cx/tam_tile_w;
	int cyl=cy/tam_tile_h;

	//CScreen::rotate(40,1.0,1.0,0.0);

	CScreen::pushMatrix();

	CScreen::translate((float)(-cx%tam_tile_w),(float)(-cy%tam_tile_h),0.0);

	for (;capa<Layer.size() && !capaSuperior(Layer[capa]);capa++) {
		Layer[capa]->refresh(cxl,cyl);
	}

	CScreen::popMatrix();

	float aux_w=floor(((float)area->getW())/((float)mapa->getTileW())+0.9);
	float aux_h=floor(((float)area->getH())/((float)mapa->getTileH())+0.9);
	int tam_w=  ((int)aux_w)+1;
	int tam_h= ((int)aux_h)+1;

	CPoint offset(cx,cy);

	ActorScrollCollection*** p_MA = mapa->MA[id];

	for (int My=cyl,Myf=cyl+tam_h;My < Myf;My++) {
#ifndef ACTORES_POR_ORDEN_EXACTO
		for (int Mx=cxl,Mxf=cxl+tam_w;Mx < Mxf;Mx++) {
			ActorScrollCollection* this_col = p_MA[Mx][My];
			this_col->sort(CActorScrollMap::orderReferenceActors);
			for (ActorScrollCollection::iterator it=this_col->begin();it!=this_col->end();++it) {
				(*it)->draw(offset);
			}
		}
#else
		ActorScrollCollection buffer;
		for (int Mx=cxl,Mxf=cxl+tam_w;Mx < Mxf;Mx++) {
			ActorScrollCollection* this_col = p_MA[Mx][My];
			buffer.insert(buffer.end(),this_col->begin(),this_col->end());
		}
		buffer.sort(CActorScrollMap::orderReferenceActors);
		for (ActorScrollCollection::iterator it=buffer.begin();it!=buffer.end();++it) {
			(*it)->draw(offset);
		}
		buffer.clear();
#endif
	}

	CScreen::pushMatrix();

	CScreen::translate((float)(-cx%tam_tile_w),(float)(-cy%tam_tile_h),0.0);

	for (;capa<Layer.size();capa++) {
		Layer[capa]->refresh(cxl,cyl);
	}

	CScreen::popMatrix();

	
}

LayerBGCollection& CFloor::getLayers() {
	return Layer;
}

#ifdef MENSAJES_MSGIDS
int CFloor::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
	printf("Piso :: ");
	return CMessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
}
#endif
