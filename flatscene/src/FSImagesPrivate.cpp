#include "FSLibrary.h"
#include "FSImagesPrivate.h"
#include "FSScreenImpl.h"

namespace flatscene {

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

Spriteset::SpritesetImpl* ImagesPrivate::add(const std::string& name,unsigned char mode) {
    Spriteset::SpritesetImpl* sptset = search(name);
    if (nullptr == sptset) {
        sptset = new Spriteset::SpritesetImpl(name,mode);
        count.at(sptset) = 0;
    } else {
        count.at(sptset)++;
    }
    return sptset;
}

void ImagesPrivate::remove(SpritesetImpl* sptset) {
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

Spriteset::SpritesetImpl* ImagesPrivate::search(const std::string& name, unsigned char mode) const {
    for (auto& pair : count)
        if (pair.first->getName() == name && pair.first->getMode() == mode)
            return pair.first;

    return nullptr;
}

std::size_t ImagesPrivate::size() const {
    return count.size();
}

unsigned int ImagesPrivate::getCount(SpritesetImpl* sptset) const {
    assert(sptset);
    const auto it = count.find(sptset);
    assert(it != count.end());
    assert(it->second > 0);
    return it->second;
}

} // flatscene
