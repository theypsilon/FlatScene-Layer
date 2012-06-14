#include "SmallTile.h"

std::vector<unsigned short>                     SmallTile::_graphCount;
std::vector<unsigned short>                     SmallTile::_collisionCount;
std::unordered_map<unsigned int,const void*>    SmallTile::_pointerMapper;
