//include guards
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//include message handler(base class)

#include "FSScreen.h"
#include "FSEngine.h"

#include "FSImages.h"
#include "FSMultiverse.h"
#include "FSWriter.h"
#include "FSTime.h"

#include "FSCamera.h"
#include "FSUniverse.h"
#include "FSActor.h"

#include "FSPimpl.h"
#include "FSSingleton.h"
#include "FSNoncopyable.h"

#include <memory>

namespace flatscene {

    enum TypeError {
        TE_standard,
        TE_fileExists,
        TE_controlViolation,
        TE_SDL_NOMSG,
        TE_SDL_MSG,
        TE_OPENGL_NOMSG,
        TE_OPENGL_MSG
    };


    class FSLibrary : private FSNoncopyable, public FSSingleton<FSLibrary> {
        friend class FSSingleton<FSLibrary>;
    public:

        int startLibrary(bool xmlconfig);

        int startLibrary( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;

        std::vector<std::unique_ptr<FSEngine>> processEngines();
        void processEngine(std::vector<std::unique_ptr<FSEngine>>& veng);
        std::vector<std::unique_ptr<FSEngine>> processEngine(std::unique_ptr<FSEngine>&& eng);


        inline FSLibrary& getLibrary() {
            return I();
        }

        const FSEngine *const getActualEngine();

        int addEngine(std::unique_ptr<FSEngine> engine,int priority);

        void exit();
        void restart();
        void runEngine(FSEngine* engine);
        void reloadEngine(FSEngine* engine);
        void changeEngine();
        void killEngine(FSEngine* engine);

        void Error (const char*,TypeError e=TE_standard);
        void Error (std::string,TypeError e=TE_standard);
        void Error (char*,TypeError e=TE_standard);

        std::string readLastError();
        std::string popError();

    #ifdef DEBUGTEST

        void debug(bool startdebug, const char* warning=NULL);
        bool inDebug();

    #endif


    private:
        FSLibrary();
        virtual ~FSLibrary();

        struct LibraryImpl;
        FSPimpl<LibraryImpl> _impl;

        friend class FSEngine;
    };
    #ifdef GLOBAL_SINGLETON_REFERENCES
    extern FSLibrary& FSLib;
    #endif

    #define SINERROR "|-| No error"

} // flatscene

#ifndef FS_AVOID_ALIAS
namespace fs = flatscene;
#endif

#endif //#ifndef __APPLICATION_H__
