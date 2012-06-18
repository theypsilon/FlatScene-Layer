//include guards
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//include message handler(base class)

#include "Includes.h"

#include "Screen.h"
#include "Engine.h"

#include "Images.h"
#include "Multiverse.h"
#include "Writer.h"
#include "Time.h"

#include "Camera.h"
#include "Universe.h"
#include "Actor.h"

#include "Exception.h"

#include "Pimpl.h"
#include "Singleton.h"
#include "Noncopyable.h"

#include <memory>

namespace FlatScene {

    enum TypeError {
        TE_standard,
        TE_fileExists,
        TE_controlViolation,
        TE_SDL_NOMSG,
        TE_SDL_MSG,
        TE_OPENGL_NOMSG,
        TE_OPENGL_MSG
    };


    class Library : private Util::NonCpmvable, public Util::Singleton<Library> {
        friend class Util::Singleton<Library>;
    public:

        int startLibrary(bool xmlconfig);

        int startLibrary( int width , int height , int bpp , bool fullscreen, bool doublebuff=true ) ;

        std::vector<std::unique_ptr<Engine>> processEngines();
        void processEngine(std::vector<std::unique_ptr<Engine>>& veng);
        std::vector<std::unique_ptr<Engine>> processEngine(std::unique_ptr<Engine>&& eng);


        inline Library& getLibrary() {
            return I();
        }

        const Engine *const getActualEngine();

        int addEngine(std::unique_ptr<Engine> engine,int priority);

        void exit();
        void restart();
        void runEngine(Engine* engine);
        void reloadEngine(Engine* engine);
        void changeEngine();
        void killEngine(Engine* engine);

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
        Library();
        virtual ~Library();

        struct LibraryImpl;
        Util::Pimpl<LibraryImpl> _impl;

        friend class Engine;
    };
    #ifdef GLOBAL_SINGLETON_REFERENCES
    extern Library& FSLib;
    #endif

    #define SINERROR "|-| No error"

} // flatscene

#endif //#ifndef __APPLICATION_H__
