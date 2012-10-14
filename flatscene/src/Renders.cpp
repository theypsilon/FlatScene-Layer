#include "Renders.h"
#include "glAdapter.h"
#include "ScreenImpl.h"

namespace FlatScene {

void SRenderLocation::operator()() {
    //glViewport(posx*m_ScaleX,posy*m_ScaleY,width*m_ScaleX,height*m_ScaleY);
    glViewport((int)posx,(int)posy,(int)width,(int)height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    static const TypeRendeProjection& trp = Screen::I()._impl->trp;

    if (trp == TRP_ORTHO) {

        //Opción ortogonal
        //glOrtho( 0.0, (double)width*m_ScaleX, (double)height*m_ScaleY, 0.0, 0.0, 1.0 ); //Los 2 últimos valores son la profundidad, sustituir por -100.0 y 100.0 para darle algo.

        glOrtho( 0.0, (double)width, (double)height, 0.0, 0.0, 1.0 ); //Los 2 últimos valores son la profundidad, sustituir por -100.0 y 100.0 para darle algo.

    } else {

        static const Float& m_maxZ = Screen::I()._impl->m_maxZ;

        //Opción de perspectiva 1
        gluPerspective(90.0f,width/height,1.0,m_maxZ);
        glTranslate(-width/2,height/2,-240.0);
        glRotate(180.0,1.0,0.0,0.0);

        //Opción de perspectiva 2
        //gluPerspective(60.0f,width/height,1.0,400.0);
        //glTranslatef(0.0,0.0,-205.0);
        //glTranslatef(0.0,0.0,101.0);
        //glRotatef(180.0,1.0,0.0,0.0);
        //glTranslatef(-width/2.0,-height/2.0,0.0);

        //Opción de perspectiva 3
        //gluPerspective(45.0f,width/height,1.0,400.0);
        //glTranslatef(0.0,0.0,-290.0);
        //glRotatef(180.0,1.0,0.0,0.0);
        //glTranslatef(-width/2.0,-height/2.0,0.0);

        //glRotatef(-45.0,1.0,0.0,0.0); //Ejemplo de rotación del plano en perspectiva
        //glTranslatef(0.0,-100.0,120.0);
    }
    delete this;
}

void SRenderTranslation::operator()() {
    glTranslate(x,y,z);
    delete this;
}

void SRenderScalation::operator()() {
    glScale(x,y,z);
    delete this;
}

void SRenderRotation::operator()() {
    glRotate(angle,x,y,z);
    delete this;
}

void SRenderColor::operator()() {
    glColor4(red,green,blue,alpha);
    delete this;
}

void SRenderPushMatrix::operator()() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void SRenderPopMatrix::operator()() {
    glPopMatrix();
}

void SRenderCanvas::operator()() {
    auto tex  = bm.getTex(),
         h2   = bm.getH(),
         w2   = bm.getW();

    auto relH = bm.getRelH(),
         relW = bm.getRelW();
         
    glBindTexture(GL_TEXTURE_2D, tex);

    glBegin(GL_QUADS);
    switch(flags) {
        case 0:
            glTexCoord2(0.0f, relH);
            glVertex2(0, h2);
            glTexCoord2(relW, relH);
            glVertex2(w2, h2);
            glTexCoord2(relW, 0.0f);
            glVertex2(w2, 0);
            glTexCoord2(0.0f, 0.0f);
            glVertex2(0,0);
            break;
        case 1:
            glTexCoord2(relW, relH);
            glVertex2(0, h2);
            glTexCoord2(0.0f, relH);
            glVertex2(w2, h2);
            glTexCoord2(0.0f, 0.0f);
            glVertex2(w2, 0);
            glTexCoord2(relW, 0.0f);
            glVertex2(0,0);
            break;
        case 2:
            glTexCoord2(0.0f, 0.0f);
            glVertex2(0, h2);
            glTexCoord2(relW, 0.0f);
            glVertex2(w2, h2);
            glTexCoord2(relW, relH);
            glVertex2(w2, 0);
            glTexCoord2(0.0f, relH);
            glVertex2(0,0);
            break;
        case 3:
            glTexCoord2(relW, 0.0f);
            glVertex2(0, h2);
            glTexCoord2(0.0f, 0.0f);
            glVertex2(w2, h2);
            glTexCoord2(0.0f, relH);
            glVertex2(w2, 0);
            glTexCoord2(relW, relH);
            glVertex2(0,0);
            break;
        default:
            break;
    }
    glEnd();

    delete this;
}

} // FlatScene