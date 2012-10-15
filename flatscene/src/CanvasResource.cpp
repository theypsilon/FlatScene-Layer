#include "CanvasResource.h"
#include "Renders.h"
#include "ScreenImpl.h"

namespace FlatScene {
    void CanvasResource::put(Float x, Float y, unsigned char flags) const {
        auto& gm = Screen::I()._impl->graphicMaterial;

        Screen::I().pushMatrix();
        Screen::I().translate(x,y,0);

        // USER DEFINED EFFECTS IN

        call_to_all(this->_initCallbackList);
        this->_initCallbackList.clear();

        gm.push_back(
            new SRenderCanvas(_gpu,flags)
        );

        // USER DEFINED EFFECTS OUT

        call_to_all(this->_endCallbackList);
        this->_endCallbackList.clear();

        Screen::I().popMatrix();
    }

    void CanvasResource::setPixel(BitmapHandler::SizeType x, BitmapHandler::SizeType y, BitmapHandler::PixelType p) {
        _gpu.setCPUBuffer()[y*getW() + x] = p;
        if (_gpu.inGPU())
            _gpu.copyToGPU();
    }

    void CanvasResource::modifyPixels(std::function<void(BitmapHandler::PAType&)> pred, bool flushchanges) {
        assert(_gpu.setCPUBuffer().size() != getW()*getH());
        pred(_gpu.setCPUBuffer());
        if (_gpu.setCPUBuffer().size() != getW()*getH())
            throw Exception("You can not modify the size of the pixel buffer");

        if (flushchanges && _gpu.inGPU())
            _gpu.copyToGPU();
    }

    void CanvasResource::modifyPixels(const BitmapHandler::PAType& buf, bool flushchanges) {
        assert(_gpu.setCPUBuffer().size() != getW()*getH());
        if (buf.size() != getW()*getH())
            throw Exception("You can not modify the size of the pixel buffer");
            
        _gpu.setCPUBuffer() = buf;

        if (flushchanges && _gpu.inGPU())
            _gpu.copyToGPU();
    }
} // FlatScene