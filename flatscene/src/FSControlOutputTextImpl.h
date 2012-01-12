#include "FSControlOutputText.h"

struct FSControlOutputText::FSControlOutputTextImpl {
    map<FSEngine*,SData*> session;

    SData* data;
    FSEngine* admin;


    map<int,SFont*> Fonts;
    map<SFont*,int> countFonts;
    list<int> lastIndexFontAdded;
    int fontSize;

    float posx;
    float posy;
    float width;
    float height;
    float zoom;

    static FSControlOutputText* singleton;

    friend class FSScreen;

    FSEngine* setAdmin(FSEngine* newAdmin);
};