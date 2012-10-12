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
} // FlatScene