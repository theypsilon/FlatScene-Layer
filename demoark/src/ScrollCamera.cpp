#include "ScrollCamera.h"
#include "FSLibrary.h"
#include "FSScreen.h"

CScrollCamera::CScrollCamera(FSActor* target, FSRectangle* area)
: FSCamera(target,area), centro(new FSPoint(area->getW()/2,area->getH()/2)) {
    intraMargenX=intraMargenY=0;
    loadUniverse();
}

CScrollCamera::CScrollCamera(FSActor* target, FSRectangle* area, FSPoint* centro, float margenDesp)
: FSCamera(target,area) {
    zoom=1.0;

    if (centro==NULL) {
        centro=this->centro=new FSPoint(area->getW()/2,area->getH()/2);
    } else {
        this->centro=centro;
        centro->x=centro->x%area->getW();
        centro->y=centro->y%area->getH();
    }
    if (margenDesp >= 0.9) {
        margenDesp = 0;
    }
    intraMargenX=(int)((float)area->getW()*margenDesp);

    if (intraMargenX > centro->getX()) {
        intraMargenX = centro->getX();
    } else if (intraMargenX > (area->getW() - centro->getX())) {
        intraMargenX = area->getW() - centro->getX();
    }
    intraMargenY=(int)((float)area->getH()*margenDesp);
    if (intraMargenY > centro->getY()) {
        intraMargenY = centro->getY();
    } else if (intraMargenY > (area->getH() - centro->getY())) {
        intraMargenY = area->getH() - centro->getY();
    }
    loadUniverse();
}

CScrollCamera::~CScrollCamera() {
    delete centro;
    FloorCollection::iterator it ;
    CFloor* f ;
    while ( !Pisos.empty ( ) )
    {
        it = Pisos.begin ( ) ;
        f = *it ;
        Pisos.erase ( it ) ;
        delete f ;
    }
}

bool CScrollCamera::focusActor(FSActor *actAct) {
    bool focused=false;

    CActorScrollMap* actScroll = dynamic_cast<CActorScrollMap*>(actAct);

    if (actScroll && (actScroll->getUniverse() == uni) && (uni != NULL)) {

        int cx=(CX()-centro->getX());
        int cy=(CY()-centro->getY());

        focused = (actScroll->m_Scrollxy.getX() >= cx -32) && (actScroll->m_Scrollxy.getX() <= (cx+32+area->getW())) &&
            (actScroll->m_Scrollxy.getY() >= cy-32) && (actScroll->m_Scrollxy.getY() <= (cy +32+ area->getH()));

    }
    return focused;
}

int CScrollCamera::loadUniverse() {

    CMap* map = (CMap*) uni;

    if (map != target->getUniverse()) {
        map= (CMap*)(uni=target->getUniverse());
        CX()=-1000;
        CY()=-1000;
    }

    if (map==NULL) FSLib.Error("map nulo.");

    if (!map->isLoaded())
        map->load();

    map->incCameras();

    Uint32* PisoCapa=map->getLayerFloor();
    Uint32* TipoCapa=map->getLayerType();

    TileBG*** Tiles=map->getTiles();

    int capas=map->getLayer();
    for (int i=0,j=0;i<=PisoCapa[capas-1] && i<capas && j<capas;i++) {
        Pisos.push_back(new CFloor(area,map,i));
        for (;j<capas && i==PisoCapa[j];j++) {
            CLayerBG* LayerAux;
            if (TipoCapa[j]==0)
                LayerAux=(CLayerBG*)new CLayerFondo(map,area,Tiles[j]);
            else if (TipoCapa[j]==1)
                LayerAux=(CLayerBG*)new CLayerInf(map,area,Tiles[j]);
            else
                LayerAux=(CLayerBG*)new CLayerSup(map,area,Tiles[j]);
            Pisos[i]->Layer.push_back(LayerAux);
        }
    }

    return EXITO;
}

int CScrollCamera::unloadUniverse() {
    if (uni!=NULL) {
        uni->decCameras();
        uni=NULL;

        FloorCollection::iterator it ;
        CFloor* f ;
        while ( !Pisos.empty ( ) )
        {
            it = Pisos.begin ( ) ;
            f = *it ;
            Pisos.erase ( it ) ;
            delete f ;
        }

        return EXITO;
    }

    return FSCamera::unloadUniverse();
}

int CScrollCamera::refresh() {

    CMap* map = (CMap*) uni;
    
    CActorScrollMap* scrolltarget = (CActorScrollMap*) target;

    int mov_x = CX() - scrolltarget->m_Scrollxy.x;

    if (abs( mov_x ) > intraMargenX) {
        if ( mov_x < 0 ) {
            CX()=scrolltarget->m_Scrollxy.x-intraMargenX;
        } else {
            CX()=scrolltarget->m_Scrollxy.x+intraMargenX;
        }

        if ( CX() < centro->getX() + map->getTileW()) { // La camara no enfoca los bordes del map.
            CX() = centro->getX()+ map->getTileW();
        } else  if ( CX() > map->getW()*map->getTileW() - area->getW() + centro->getX() - map->getTileW()) { // Quiz�s ser�a m�s apropiado restarle adicionalmente 1 unidad a los margenes exteriores del map (derecha y abajo).
            CX() = map->getW()*map->getTileW() - area->getW() + centro->getX() - map->getTileW() ;
        }
    }

    int mov_y=CY()-scrolltarget->m_Scrollxy.y;

    if (abs(mov_y) > intraMargenY) {
        if ( mov_y < 0 ) {
            CY()=scrolltarget->m_Scrollxy.y-intraMargenY;
        } else {
            CY()=scrolltarget->m_Scrollxy.y+intraMargenY;
        }

        if ( CY() < centro->getY() + map->getTileH() ) {
            CY() = centro->getY() + map->getTileH();
        } else  if ( CY() >  map->getH()*map->getTileH()- area->getH() + centro->getY() - map->getTileH()) {
            CY() = map->getH()*map->getTileH() - area->getH() + centro->getY() - map->getTileH() ;
        }
    }

    scrolltarget->renderPoint.x = scrolltarget->m_Scrollxy.x -(CX() - (area->getW()/2));  
    scrolltarget->renderPoint.y = scrolltarget->m_Scrollxy.y -(CY() - (area->getH()/2));

#ifdef MENSAJES_COORDENADAS
    Write.erase(Write.line(0,area->getX()*2+area->getW()*2-300,area->getY()*2+area->getH()*2-60,"Scroll X: %d    Scroll Y: %d", scrolltarget->m_Scrollxy.x,scrolltarget->m_Scrollxy.y),true);
    Write.erase(Write.line(0,area->getX()*2+area->getW()*2-300,area->getY()*2+area->getH()*2-30,"Camera X: %d    Camera Y: %d",CX(),CY()),true);
    
#endif


#ifdef __TESTZOOM__
    if (zoom < 1.0) {
        FSDraw.translate(-centro->getX()*zoom+centro->getX(),-centro->getY()*zoom+centro->getY(),0.0);
        FSDraw.rotate((1.0-zoom)*8,0.0,0.0,1.0);
    }
    static float factorzoom=0.001;
    if (zoom>1.5) {
        factorzoom=-0.002;
    } else if (zoom < 0.10) {
        factorzoom=0.002;
    }
    if (factorzoom<0) {
    setZoom(zoom/1.01);
    } else {
    setZoom(zoom*1.01);
    }
#endif

    FSDraw.scale(2.0,2.0,1.0);

    //CScreen::translate(320.0/2,240.0/2,-120.0);

    for (int i=0;i<Pisos.size();i++) {
        Pisos[i]->refresh(CX()-centro->getX(),CY()-centro->getY());
    }


    return EXITO;

}

void CScrollCamera::setZoom(float newZoom) {
    zoom=newZoom;
    for (int i=0;i<Pisos.size();i++) {
        LayerBGCollection& Layer=Pisos[i]->getLayers();
        for (int j=0;j<Layer.size();j++) {
            Layer[j]->init(zoom);
        }
    }
}

#ifdef MENSAJES_MSGIDS
int CScrollCamera::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
    printf("ScrollCamera %d.%d :: ",area->getX(),area->getY());
    return FSMessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
}
#endif
