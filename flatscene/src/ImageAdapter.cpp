#include "CanvasResource.h"
#include "Exception.h"
#include <iostream>

namespace FlatScene {

    CanvasResource::~CanvasResource() {
        cleanResourcesCPU();
        cleanResourcesGPU();
    }

    void CanvasResource::cleanResourcesGPU() {
        if (tex) {
            glDeleteTextures( 1, &tex );
        }
        tex = 0;
    }

    void CanvasResource::cleanResourcesCPU() {
        if (raw)
            SDL_FreeSurface(raw);
        raw = nullptr;
    }


    unsigned int getPixel(unsigned int x, unsigned int y, RawImageResource raw) {
        if (!raw)
            throw Exception("Pixel data is not accesible for this configuration.");

        if ((int) x > raw->w || (int) y > raw->h)
            throw Exception("Wrong coordinates for getting a pixel from this surface.");

        unsigned int color = 0;
        int position = y * raw->pitch + raw->format->BytesPerPixel * x ;
        char* buffer = (char*) raw->pixels ;
        buffer += position ;
        memcpy ( &color , buffer , raw->format->BytesPerPixel ) ;
        return color;
    }
} //FlatScene