#include "TestOSDesktopInterface.h"

CTestOSDesktopInterface::CTestOSDesktopInterface(FSMessageHandler * pmhParent) {
    raton = fondo = NULL;
    camaraf = camarar = NULL;

    xpos = ypos = zpos = xrot = yrot = 0.0;

    lastx = 400.0;
    lasty = 300.0;

    up = down = left = right = false;

    text = -1;
    file = -1;
}
CTestOSDesktopInterface::~CTestOSDesktopInterface() {

}

int CTestOSDesktopInterface::onInit() {

    fondo = new CActorMouse(this);

    camaraf = new CCameraOS(fondo, new FSRectangle(0,0,FSDraw.getWidth(),FSDraw.getHeight()),this);

    FSDraw.projectionMode(TRP_PERSPECTIVE,1000);

    raton = new CActorMouse(this);

    camarar = new CCameraOS(raton, new FSRectangle(0,0,FSDraw.getWidth(),FSDraw.getHeight()),this);

    fondo->file = Img.add("flower-wallpaper-800x600.jpg");
    fondo->graph = 0;
        
    raton->file = Img.add("Raton");
    raton->graph = 0;

    fondo->renderPoint.set(-400,0);

    return FSEngine::onInit();
}

int CTestOSDesktopInterface::loop() {

    if (FSDraw.changeScreen(800,600,32,1.0,1.0,false) == FRACASO)
        FSDraw.start(800,600,32,1.0,1.0,false);


    Write.erase();


    Write.line(0,500,380,"Pulsa F1 para cambiar de Test");
    Write.line(0,500,410,"Test OS");
    Write.line(0,500,440,"cursores : desplazarse");
    Write.line(0,500,470,"raton : inclinacion");
    
    
    FSEngine::loop();

    FSDraw.changeScreen(640,480,32,2.0,2.0,false);



    return EXITO;

}

int CTestOSDesktopInterface::onExit() {
    if (raton) {
        Img.remove(raton->file);
        delete raton;
    }

    if (fondo) {
        Img.remove(fondo->file);
        delete fondo;
    }

    if (camarar)
        delete camarar;

    if (camaraf)
        delete camaraf;

    return EXITO;

}

int CTestOSDesktopInterface::drawFrame() {

    camaraf->locateRenderScene();

    camaraf->translate(800.0,300.0,0.0);

    camaraf->rotate(xrot,1.0,0.0,0.0);  //rotate our camera on teh 
    camaraf->rotate(yrot,0.0,1.0,0.0);  //rotate our camera on the 

    camaraf->translate(-xpos,-ypos,-zpos); //translate the screen

    camaraf->render();

    camarar->locateRenderScene();

    camarar->translate(400.0,300.0,0.0);

    camarar->render();

    Write.render();

    return EXITO;
}

int CTestOSDesktopInterface::onIdle() {

    printf("x : %d        y : %d\r",raton->renderPoint.x,raton->renderPoint.y);

/*	if (right)  {
        float yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xpos += float(cos(yrotrad)) * 2.0;
        zpos += float(sin(yrotrad)) * 2.0;
    }

    if (left) {

        float yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xpos -= float(cos(yrotrad)) * 2.0;
        zpos -= float(sin(yrotrad)) * 2.0;

    }

    if (up) {

        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xrotrad = (xrot / 180 * 3.141592654f); 
        xpos -= float(sin(yrotrad))  * 2.0;
        zpos += float(cos(yrotrad))  * 2.0;
        ypos += float(sin(xrotrad))  * 2.0;

    }

    if (down) {

        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xrotrad = (xrot / 180 * 3.141592654f); 
        xpos += float(sin(yrotrad)) * 2.0;
        zpos -= float(cos(yrotrad))  * 2.0;
        ypos -= float(sin(xrotrad)) * 2.0;

    }*/

    return EXITO;

}

void CTestOSDesktopInterface::onKeyTestOS(SDL_Event* event) {

    CTestOSDesktopInterface * os =  (CTestOSDesktopInterface*) FSLib.getActualEngine();
    SDLKey key = event->key.keysym.sym;

    if (event->type == SDL_KEYDOWN) {

        if (key == SDLK_LEFT) {

            os->left = true;

        } else if (key == SDLK_RIGHT) {

            os->right = true;

        } else if (key == SDLK_UP) {

            os->up = true;

        } else if (key == SDLK_DOWN) {

            os->down = true;

        } else if (key==SDLK_F1) 
            FSLib.getLibrary().SendMessage(FSLib.MSGID_ChangeEngine);

    } else if (event->type == SDL_KEYUP) {

        if (key == SDLK_LEFT) {

            os->left = false;

        } else if (key == SDLK_RIGHT) {

            os->right = false;

        } else if (key == SDLK_UP) {

            os->up = false;

        } else if (key == SDLK_DOWN) {

            os->down = false;

        }

    }
}

void CTestOSDesktopInterface::onMouseTestOS(SDL_Event* event) {

    CTestOSDesktopInterface * os =  (CTestOSDesktopInterface*) FSLib.getActualEngine();

    int x = os->raton->renderPoint.x = event->motion.x;
    int y = os->raton->renderPoint.y = event->motion.y;

    if (os->lastx != 0.0) {

        int diffx=x-os->lastx;
        int diffy=y-os->lasty;

        os->xrot += (float) diffy / 10.0; 
        os->yrot += (float) diffx / 10.0;    

    }

    os->lastx=x; 
    os->lasty=y;


}
