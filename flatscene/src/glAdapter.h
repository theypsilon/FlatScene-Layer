#include "FSIncludes.h"

namespace flatscene {

    inline void glTranslate(Float x, Float y, Float z) {
        glTranslatef((GLfloat)x,(GLfloat)y,(GLfloat)z);
    }

    inline void glScale(Float x, Float y, Float z) {
        glScalef((GLfloat)x,(GLfloat)y,(GLfloat)z);
    }

    inline void glRotate(Float angle, Float x, Float y, Float z) {
        glRotatef((GLfloat)angle,(GLfloat)x,(GLfloat)y,(GLfloat)z);
    }

    inline void glColor4(Float red, Float green, Float blue, Float alpha) {
        glColor4f((GLfloat)red,(GLfloat)green,(GLfloat)blue,(GLfloat)alpha);
    }

    inline void glTexCoord2(Float s, Float t) {
        glTexCoord2f((GLfloat)s,(GLfloat)t);
    }

    inline void glVertex2(Float x, Float y) {
        glVertex2f((GLfloat)x,(GLfloat)y);
    }

}