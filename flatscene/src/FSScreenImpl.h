#ifndef __FS_SCREEN_IMPL__
#define __FS_SCREEN_IMPL__

#include "FSScreen.h"

struct SRender {
	virtual void operator()()=0;
};

struct SRenderLocation : SRender { // LOCATE
	float posx;
	float posy;
	float width;
	float height;
	float zoom;

	void operator()();
};

struct SRenderTranslation : SRender { // TRANSLATE && SCALATION
	float x;
	float y;
	float z;

	void operator()();
};

struct SRenderScalation : SRender { // TRANSLATE && SCALATION
	float x;
	float y;
	float z;

	void operator()();
};

struct SRenderRotation : SRender { // ROTATION
	float angle;
	float x;
	float y;
	float z;

	void operator()();
};

struct SRenderColor : SRender { // ROTATION
	float red;
	float green;
	float blue;
	float alpha;

	void operator()();
};

struct SRenderCanvasFloat : SRender {
	SCanvas canvas;
	FSFloatPoint ptDst;
	Uint8 flags;

	SRenderCanvasFloat(SCanvas canvas, FSFloatPoint ptDst, Uint8 flags)
	: canvas(canvas), ptDst(ptDst), flags(flags) {
	}

	void operator()();
};

struct SRenderCanvasInt : SRender {
	SCanvas canvas;
	FSPoint ptDst;
	Uint8 flags;

	SRenderCanvasInt(SCanvas canvas, FSPoint ptDst, Uint8 flags)
	: canvas(canvas), ptDst(ptDst), flags(flags) {
	}

	void operator()();
};


struct SRenderPushMatrix : SRender { // ROTATION
	void operator()();
};

struct SRenderPopMatrix : SRender { // ROTATION
	void operator()();
};

struct FSScreen::ScreenImpl {

    SDL_Surface* m_SDL_Surface;

    bool rendering;
    TypeRendeProjection trp;

    float m_maxZ;

    bool m_FullScreen,m_Doublebuff;

    int m_Width,m_Height,m_Bpp;

    list<SRender*> graphicMaterial;

    list<FSSprite*> spriteToDelete;
    list<FSSpriteset*> spritesetToDelete;
    list<FSCanvas*> imageToDelete;

    void deleteResources();

    void saveResources(GraphicResources& info);
    void reloadResources(GraphicResources& info);


    float red,green,blue,alpha;

    int beginRenderMode(Uint32 flags);
    int endRenderMode(Uint32 flags);

};

#endif
