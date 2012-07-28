#ifndef _I_DEBUG_
#define _I_DEBUG_

#include "IInclude.h"

int func_fracaso(const char* file, int line);

#define AVOID_FRACASO

#ifdef AVOID_FRACASO
#undef FRACASO
#define FRACASO func_fracaso(__FILE__,__LINE__);
#endif

void assert_text(std::string cad,int line);

#endif
