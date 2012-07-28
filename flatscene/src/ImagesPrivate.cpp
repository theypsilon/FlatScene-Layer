#include "ImagesPrivate.h"
#include "SpriteSetResource.h"
//#include "ScreenImpl.h"

#include <iostream>

namespace FlatScene {

ImagesPrivate::ImagesPrivate() {}

ImagesPrivate::~ImagesPrivate() {
    clear();
}

void ImagesPrivate::clear() {

    bool needSave = nullptr != SDL_GetVideoSurface();
    for(auto& pair : count)
        if (needSave)
            ;//Screen::I()._impl->spritesetToDelete.push_back(std::move(it.first));
        else
            delete pair.first;

    count.clear();
}

SpritesetResource* ImagesPrivate::add(const std::string& name,unsigned char mode) {
    SpritesetResource* sptset = search(name);
    if (nullptr == sptset) {
        sptset = new SpritesetResource(name,mode);
        count.insert(std::make_pair(sptset,1));
    } else {
        count.at(sptset)++;
    }
    return sptset;
}

void ImagesPrivate::remove(SpritesetResource* sptset) {
    assert(sptset);
    auto it = count.find(sptset);
    assert(it != count.end());
    assert(it->second > 0);

    if (1 == it->second) {
        delete it->first;
        count.erase(it);
    } else {
        it->second--;
    }
}

SpriteResource* ImagesPrivate::searchSprite(SDL_Surface* surf,unsigned int pos, unsigned char mode) const {
    void* ptr = reinterpret_cast<void*>(surf);
    auto it = mapSprite.find(ptr);
    if (it == end(mapSprite))
        return nullptr;
    auto et = it->second.find(pos);
    if (et == end(it->second))
        return nullptr;

    return et->second;
}

SpriteResource* ImagesPrivate::addSprite(SDL_Surface* surf,unsigned int pos, unsigned char mode) {
    SpriteResource* ret = searchSprite(surf,pos,mode);
    if (ret != nullptr) {
        countSprite[ret]++;
        return ret;
    }
    ret = new SpriteResource();
    countSprite.emplace(std::make_pair(ret,1));
    mapSprite[reinterpret_cast<void*>(surf)][pos] = ret;
    return ret;
}

void ImagesPrivate::removeSprite(SpriteResource* sptset) {
    using namespace std;
    auto it = countSprite.find(sptset);
    if (it->second == 0) {
        bool found = false;
        for (auto& pair : mapSprite) {
            for (auto& subpair : pair.second) {
                found = subpair.second == sptset;
                if (found) {
                    pair.second.erase(subpair.first);
                    break;
                }
            }
        }
        assert(found);
        countSprite.erase(sptset);
        delete sptset;
    } else { 
        assert(it->second > 0);
    }
}

SpritesetResource* ImagesPrivate::search(const std::string& name, unsigned char mode) const {
    for (auto& pair : count)
        if (pair.first->getName() == name && pair.first->getMode() == mode  )
            return pair.first;

    return nullptr;
}

std::size_t ImagesPrivate::size() const {
    return count.size();
}

unsigned int ImagesPrivate::getCount(SpritesetResource* sptset) const {
    assert(sptset);
    const auto it = count.find(sptset);
    assert(it != count.end());
    assert(it->second > 0);
    return it->second;
}

} // flatscene
