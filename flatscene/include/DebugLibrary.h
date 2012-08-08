#ifndef FS_DEBUG_LIBRARY__
#define FS_DEBUG_LIBRARY__

/*
    The functions declared in these sections are not
    intended to be used by library clients.

    This can be suddenly deleted in any version of the
    library, and it is only here for testing purposes.

    Unit Tests will make this unnecesary in the future.
*/

namespace FlatScene { namespace Debug {

    void doDebug0();

    void doDebug1();

}} // FlatScene::Debug


#endif // FS_DEBUG_LIBRARY__