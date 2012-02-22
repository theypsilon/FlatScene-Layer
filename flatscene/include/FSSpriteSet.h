#ifndef __SPRITESET_H__
#define __SPRITESET_H__

#include "FSSprite.h"
#include <memory>

typedef struct { 
    std::string name; 
    Uint8 mode; 
    int times;
    int number;
} SpritesetInfo ;

class FSSpriteset {
public:

    const FSSprite* get ( unsigned int n ) const;

    int size () const;

    const std::string& getName() const;

    Uint8 getMode() const;

private :
    struct SpritesetImpl;
    const std::unique_ptr<SpritesetImpl> _impl;

    FSSpriteset(std::string c = "",Uint8 mode=ONLY_TEXTURE);
    virtual ~FSSpriteset();

    friend class FSScreen;
    friend class FSImages;
};

#endif
