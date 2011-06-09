#include "FSCamera.h"
#include "FSLibrary.h"

map<TypeResource,void (*)(void*)> CCamera::procRenders;

CCamera::CCamera(CActor* target, CRectangle* area,CMessageHandler* pmhParent) :
CMessageHandler(pmhParent), uni(NULL), target(target), area(area) , rendering(false),
x(-1000),y(-1000) {

	if (procRenders.size() == 0) 
		initProcRenders();

}

CCamera::~CCamera() {

	for (list<SToRender*>::iterator iri = initRenderList.begin(), ire = initRenderList.end();iri!=ire;++iri) {

		SToRender* irp = *iri;

		CScreen::procRenders[irp->type](irp->pointer);

		delete *iri;

	}

	initRenderList.clear();


	for (list<SToRender*>::iterator eri = endRenderList.begin(), ere = endRenderList.end();eri!=ere;++eri) {

		SToRender* erp = *eri;

		CScreen::procRenders[erp->type](erp->pointer);

		delete *eri;

	}

	endRenderList.clear();

	delete area;
}

CUniverse* CCamera::getUniverse() {
	return uni;
}

int CCamera::loadUniverse() {

	if (uni != NULL) {
		CLibrary::Error("Universe already loaded");
		return FRACASO;
	}


	return EXITO;
}

int CCamera::unloadUniverse() {
	if (uni == NULL) {
		CLibrary::Error("No Universe in focus");
		return FRACASO;
	}


	uni->decCameras();
	uni=NULL;

	return EXITO;

}

int CCamera::resyncUniverse() {

	if (unloadUniverse() == EXITO)
		return loadUniverse();
	else
		return FRACASO;
}

bool CCamera::isOpened() {
	return (uni!=NULL);
}


int& CCamera::CX() {
	return x;
}
int& CCamera::CY() {
	return y;
}
CActor* CCamera::Target() {
	return target;
}

int CCamera::setTarget(CActor* newTarget) {
	if (newTarget == this->target) {
		CLibrary::Error("Actor objetivo ya establecido");
		return FRACASO;
	}

	if (newTarget->getUniverse()!= this->target->getUniverse()) {
		this->target=newTarget;
		resyncUniverse();
	} else {
		this->target=newTarget;
	}

	CX()=CY()=-1000; // Usado para forzar una recalibración de las coordenadas de la cámara en su primer uso.
	
	return EXITO;
}


CRectangle* CCamera::getArea() {
	return area;
}

int CCamera::render() {

	rendering = true;

	for (list<SToRender*>::iterator iri = initRenderList.begin(), ire = initRenderList.end();iri!=ire;++iri) {

		SToRender* irp = *iri;

		procRenders[irp->type](irp->pointer);

		delete irp;

	}

	initRenderList.clear();

	int ret = refresh();

	for (list<SToRender*>::iterator eri = endRenderList.begin(), ere = endRenderList.end();eri!=ere;++eri) {

		SToRender* erp = *eri;

		procRenders[erp->type](erp->pointer);

		delete erp;

	}

	endRenderList.clear();

	rendering = false;

	return ret;
}



int CCamera::refresh() {
	return EXITO;
}

void CCamera::initProcRenders() {

	procRenders[TR_ROTATION] = procRendRotation;
	procRenders[TR_TRANSLATION] = procRendTranslation;
	procRenders[TR_SCALATION] = procRendScalation;
	procRenders[TR_COLOR] = procRendColor;
	procRenders[TR_PUSHMATRIX] = procRendPush;
	procRenders[TR_POPMATRIX] = procRendPop;
	procRenders[TR_LOCATION] = procRendLocation;

}

void CCamera::procRendPush(void* pointer) {
	CScreen::pushMatrix();
}

void CCamera::procRendPop(void* pointer) {
	CScreen::popMatrix();
}

void CCamera::procRendLocation(void* pointer) {

	SRenderLocation* n = (SRenderLocation*) pointer;

	float posx = n->posx;
	float posy = n->posy;
	float width = n->width;
	float height = n->height;
	float zoom = n->zoom;

	delete n;

	CScreen::locateRenderScene(posx,posy,width,height,zoom);

}


void CCamera::procRendRotation(void* pointer) {

	SRenderRotation* n = (SRenderRotation*) pointer;

	GLint angle = n->angle;
	GLint x = n->x;
	GLint y = n->y;
	GLint z = n->z;

	delete n;

	CScreen::rotate(angle,x,y,z);

}

void CCamera::procRendTranslation(void* pointer) {


	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;

	GLfloat z = n->z;

	delete n;

	CScreen::translate(x,y,z);

}

void CCamera::procRendScalation(void* pointer) {

	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;

	CScreen::scale(x,y,z);


}

void CCamera::procRendColor(void* pointer) {

	SRenderColor* n = (SRenderColor*) pointer;

	GLfloat red = n->red;
	GLfloat green = n->green;
	GLfloat blue = n->blue;
	GLfloat alpha = n->alpha;

	delete n;

	CScreen::color(red,green,blue,alpha);

}

int CCamera::locateRenderScene( CRectangle* areaSc, float zoom ) {
	return locateRenderScene(areaSc->getX(),areaSc->getY(),areaSc->getW(),areaSc->getH(),zoom);
}

int CCamera::locateRenderScene( float posx, float posy, float width, float height, float zoom ) {

	if (width == 0 || height == 0) {
		posx = area->getX();
		posy = area->getY();
		width = area->getW();
		height = area->getH();
	}

	if (rendering) {

		CScreen::pushMatrix();

		CScreen::locateRenderScene(posx,posy,width,height,zoom);

	} else {

		//PUSH

		SToRender* r_init = new SToRender;

		r_init->type = TR_PUSHMATRIX;
		r_init->pointer = NULL;

		initRenderList.push_back(r_init);

		//LOCATE

		SRenderLocation* c_init = new SRenderLocation();

		c_init->posx = posx;
		c_init->posy = posy;
		c_init->width = width;
		c_init->height = height;
		c_init->zoom = zoom;

		r_init = new SToRender();

		r_init->type = TR_LOCATION;
		r_init->pointer = (void*) c_init;

		initRenderList.push_back(r_init);

	}
	
	//POP

	SToRender* r_fin = new SToRender;

	r_fin->type = TR_POPMATRIX;
	r_fin->pointer = NULL;

	endRenderList.push_front(r_fin);


	return EXITO;
}

int CCamera::rotate(float angle, float x, float y, float z) {

	if (rendering) {

		CScreen::pushMatrix();

		CScreen::rotate(angle,x,y,z);

	} else {

		//PUSH

		SToRender* r_init = new SToRender;

		r_init->type = TR_PUSHMATRIX;
		r_init->pointer = NULL;

		initRenderList.push_back(r_init);

		//SCALE

		SRenderRotation* c_init = new SRenderRotation();

		c_init->x = x;
		c_init->y = y;
		c_init->z = z;
		c_init->angle = angle;

		r_init = new SToRender();

		r_init->type = TR_ROTATION;
		r_init->pointer = (void*) c_init;

		initRenderList.push_back(r_init);

	}

	//POP

	SToRender* r_fin = new SToRender;

	r_fin->type = TR_POPMATRIX;
	r_fin->pointer = NULL;

	endRenderList.push_front(r_fin);

	return EXITO;
}
int CCamera::translate(float x, float y, float z) {

	if (rendering) {

		CScreen::pushMatrix();

		CScreen::translate(x,y,z);

	} else {

		//PUSH

		SToRender* r_init = new SToRender;

		r_init->type = TR_PUSHMATRIX;
		r_init->pointer = NULL;

		initRenderList.push_back(r_init);

		//SCALE

		SRenderTranscalation* c_init = new SRenderTranscalation();

		c_init->x = x;
		c_init->y = y;
		c_init->z = z;

		r_init = new SToRender();

		r_init->type = TR_TRANSLATION;
		r_init->pointer = (void*) c_init;

		initRenderList.push_back(r_init);

	}

	//POP

	SToRender* r_fin = new SToRender;

	r_fin->type = TR_POPMATRIX;
	r_fin->pointer = NULL;

	endRenderList.push_front(r_fin);

	return EXITO;
}
int CCamera::scale(float x, float y, float z) {

	if (rendering) {

		CScreen::pushMatrix();

		CScreen::scale(x,y,z);

	} else {

		//PUSH

		SToRender* r_init = new SToRender;

		r_init->type = TR_PUSHMATRIX;
		r_init->pointer = NULL;

		initRenderList.push_back(r_init);

		//SCALE

		SRenderTranscalation* c_init = new SRenderTranscalation();

		c_init->x = x;
		c_init->y = y;
		c_init->z = z;

		r_init = new SToRender();

		r_init->type = TR_SCALATION;
		r_init->pointer = (void*) c_init;

		initRenderList.push_back(r_init);

	}

	//POP

	SToRender* r_fin = new SToRender;

	r_fin->type = TR_POPMATRIX;
	r_fin->pointer = NULL;

	endRenderList.push_front(r_fin);

	return EXITO;
}

int CCamera::color(float red, float green, float blue, float alpha) {

	if (red > 1.0) red = 1.0;
	if (green > 1.0) green = 1.0;
	if (blue > 1.0) blue = 1.0;
	if (alpha > 1.0) alpha = 1.0;

	if (rendering) {
		CScreen::color(red,green,blue,alpha);
	} else {
		SRenderColor * c_init = new SRenderColor();

		c_init->red = red;
		c_init->green = green;
		c_init->blue = blue;
		c_init->alpha = alpha;

		SToRender* r_init = new SToRender();

		r_init->type = TR_COLOR;
		r_init->pointer = (void*) c_init;

		initRenderList.push_back(r_init);
	}

	SRenderColor * c_fin = new SRenderColor();

	c_fin->red = CScreen::red;//2.0 - red;
	c_fin->green = CScreen::green;//2.0 - green;
	c_fin->blue = CScreen::blue;//2.0 - blue;
	c_fin->alpha =  CScreen::alpha;//2.0 - alpha;

	SToRender* r_fin = new SToRender();

	r_fin->type = TR_COLOR;
	r_fin->pointer = (void*) c_fin;

	endRenderList.push_front(r_fin);

	return EXITO;
}

int CCamera::color(CColor* col, float alpha) {

	return color(((float)col->getR())/255.0,((float)col->getG())/255.0,((float)col->getB())/255.0,alpha);
}


int CCamera::reubicate(CRectangle* nArea) {
	if (area == nArea) {
		CLibrary::Error("Area ya establecida");
		return FRACASO;
	}

	delete area;
	area=nArea;

	return EXITO;

}

#ifdef MENSAJES_MSGIDS
int CCamera::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
	printf("Camera %d.%d :: ",area->getX(),area->getY());
	return CMessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
}
#endif