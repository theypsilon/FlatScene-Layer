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
        if (!_gpu.inCPU()) _gpu.copyToCPU();

        _gpu.setCPUBuffer()[y*_gpu.getW() + x] = p;
        if (_gpu.inGPU())
            _gpu.copyToGPU();
    }

    void CanvasResource::modifyPixels(std::function<void(BitmapHandler::PAType&)> pred, bool flushchanges) {
        if (!_gpu.inCPU()) _gpu.copyToCPU();

        auto& innerBuf = _gpu.setCPUBuffer();
        auto size = innerBuf.size();
        pred(innerBuf);
        if (innerBuf.size() != size)
            throw Exception("You can not modify the size of the pixel buffer");

        if (flushchanges && _gpu.inGPU())
            _gpu.copyToGPU();
    }

    void CanvasResource::replacePixels(const BitmapHandler::PAType& buf, bool flushchanges) {
        if (!_gpu.inCPU()) _gpu.copyToCPU(); 

        auto& innerBuf = _gpu.setCPUBuffer();
        if (buf.size() != innerBuf.size())
            throw Exception("You can not modify the size of the pixel buffer");
            
        innerBuf = buf;

        if (flushchanges && _gpu.inGPU())
            _gpu.copyToGPU();
    }
} // FlatScene