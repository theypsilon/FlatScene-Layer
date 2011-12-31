#include "FSCamera.h"
#include "FSLibrary.h"

map<TypeResource,void (*)(void*)> FSCamera::procRenders;

FSCamera::FSCamera(FSActor* target, FSRectangle* area,FSMessageHandler * pmhParent) :
FSMessageHandler(pmhParent), uni(NULL), target(target), area(area) , rendering(false),
x(-1000),y(-1000) {

	if (procRenders.size() == 0) 
		initProcRenders();

}

FSCamera::~FSCamera() {

	for (list<SToRender*>::iterator iri = initRenderList.begin(), ire = initRenderList.end();iri!=ire;++iri) {

		SToRender* irp = *iri;

		FSScreen::procRenders[irp->type](irp->pointer);

		delete *iri;

	}

	initRenderList.clear();


	for (list<SToRender*>::iterator eri = endRenderList.begin(), ere = endRenderList.end();eri!=ere;++eri) {

		SToRender* erp = *eri;

		FSScreen::procRenders[erp->type](erp->pointer);

		delete *eri;

	}

	endRenderList.clear();

	delete area;
}

FSUniverse* FSCamera::getUniverse() {
	return uni;
}

int FSCamera::loadUniverse() {

	if (uni != NULL) {
		FSLibrary::Error("Universe already loaded");
		return FRACASO;
	}


	return EXITO;
}

int FSCamera::unloadUniverse() {
	if (uni == NULL) {
		FSLibrary::Error("No Universe in focus");
		return FRACASO;
	}


	uni->decCameras();
	uni=NULL;

	return EXITO;

}

int FSCamera::resyncUniverse() {

	if (unloadUniverse() == EXITO)
		return loadUniverse();
	else
		return FRACASO;
}

bool FSCamera::isOpened() {
	return (uni!=NULL);
}


int& FSCamera::CX() {
	return x;
}
int& FSCamera::CY() {
	return y;
}
FSActor* FSCamera::Target() {
	return target;
}

int FSCamera::setTarget(FSActor* newTarget) {
	if (newTarget == this->target) {
		FSLibrary::Error("Actor objetivo ya establecido");
		return FRACASO;
	}

	if (newTarget->getUniverse()!= this->target->getUniverse()) {
		this->target=newTarget;
		resyncUniverse();
	} else {
		this->target=newTarget;
	}

	CX()=CY()=-1000; // Usado para forzar una recalibraci�n de las coordenadas de la c�mara en su primer uso.
	
	return EXITO;
}


FSRectangle* FSCamera::getArea() {
	return area;
}

int FSCamera::render() {

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



int FSCamera::refresh() {
	return EXITO;
}

void FSCamera::initProcRenders() {

	procRenders[TR_ROTATION] = procRendRotation;
	procRenders[TR_TRANSLATION] = procRendTranslation;
	procRenders[TR_SCALATION] = procRendScalation;
	procRenders[TR_COLOR] = procRendColor;
	procRenders[TR_PUSHMATRIX] = procRendPush;
	procRenders[TR_POPMATRIX] = procRendPop;
	procRenders[TR_LOCATION] = procRendLocation;

}

void FSCamera::procRendPush(void* pointer) {
	FSScreen::pushMatrix();
}

void FSCamera::procRendPop(void* pointer) {
	FSScreen::popMatrix();
}

void FSCamera::procRendLocation(void* pointer) {

	SRenderLocation* n = (SRenderLocation*) pointer;

	float posx = n->posx;
	float posy = n->posy;
	float width = n->width;
	float height = n->height;
	float zoom = n->zoom;

	delete n;

	FSScreen::locateRenderScene(posx,posy,width,height,zoom);

}


void FSCamera::procRendRotation(void* pointer) {

	SRenderRotation* n = (SRenderRotation*) pointer;

	GLint angle = n->angle;
	GLint x = n->x;
	GLint y = n->y;
	GLint z = n->z;

	delete n;

	FSScreen::rotate(angle,x,y,z);

}

void FSCamera::procRendTranslation(void* pointer) {


	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;

	GLfloat z = n->z;

	delete n;

	FSScreen::translate(x,y,z);

}

void FSCamera::procRendScalation(void* pointer) {

	SRenderTranscalation* n = (SRenderTranscalation*) pointer;

	GLfloat x = n->x;
	GLfloat y = n->y;
	GLfloat z = n->z;

	delete n;

	FSScreen::scale(x,y,z);


}

void FSCamera::procRendColor(void* pointer) {

	SRenderColor* n = (SRenderColor*) pointer;

	GLfloat red = n->red;
	GLfloat green = n->green;
	GLfloat blue = n->blue;
	GLfloat alpha = n->alpha;

	delete n;

	FSScreen::color(red,green,blue,alpha);

}

int FSCamera::locateRenderScene( FSRectangle* areaSc, float zoom ) {
	return locateRenderScene(areaSc->getX(),areaSc->getY(),areaSc->getW(),areaSc->getH(),zoom);
}

int FSCamera::locateRenderScene( float posx, float posy, float width, float height, float zoom ) {

	if (width == 0 || height == 0) {
		posx = area->getX();
		posy = area->getY();
		width = area->getW();
		height = area->getH();
	}

	if (rendering) {

		FSScreen::pushMatrix();

		FSScreen::locateRenderScene(posx,posy,width,height,zoom);

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

int FSCamera::rotate(float angle, float x, float y, float z) {

	if (rendering) {

		FSScreen::pushMatrix();

		FSScreen::rotate(angle,x,y,z);

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
int FSCamera::translate(float x, float y, float z) {

	if (rendering) {

		FSScreen::pushMatrix();

		FSScreen::translate(x,y,z);

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
int FSCamera::scale(float x, float y, float z) {

	if (rendering) {

		FSScreen::pushMatrix();

		FSScreen::scale(x,y,z);

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

int FSCamera::color(float red, float green, float blue, float alpha) {

	if (red > 1.0) red = 1.0;
	if (green > 1.0) green = 1.0;
	if (blue > 1.0) blue = 1.0;
	if (alpha > 1.0) alpha = 1.0;

	if (rendering) {
		FSScreen::color(red,green,blue,alpha);
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

	c_fin->red = FSScreen::red;//2.0 - red;
	c_fin->green = FSScreen::green;//2.0 - green;
	c_fin->blue = FSScreen::blue;//2.0 - blue;
	c_fin->alpha =  FSScreen::alpha;//2.0 - alpha;

	SToRender* r_fin = new SToRender();

	r_fin->type = TR_COLOR;
	r_fin->pointer = (void*) c_fin;

	endRenderList.push_front(r_fin);

	return EXITO;
}

int FSCamera::color(FSColor* col, float alpha) {

	return color(((float)col->getR())/255.0,((float)col->getG())/255.0,((float)col->getB())/255.0,alpha);
}


int FSCamera::reubicate(FSRectangle* nArea) {
	if (area == nArea) {
		FSLibrary::Error("Area ya establecida");
		return FRACASO;
	}

	delete area;
	area=nArea;

	return EXITO;

}

#ifdef MENSAJES_MSGIDS
int FSCamera::SendMessage(Uint32 MsgID,MSGPARM ParmMsg) {
	printf("Camera %d.%d :: ",area->getX(),area->getY());
	return FSMessageHandler::SendMessage(MsgID,ParmMsg,Parm2);
}
#endif