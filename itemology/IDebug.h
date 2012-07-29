#ifndef _I_DEBUG_
#define _I_DEBUG_

#include "IInclude.h"

int func_EXIT_FAILURE(const char* file, int line);

#define AVOID_EXIT_FAILURE

#ifdef AVOID_EXIT_FAILURE
#undef EXIT_FAILURE
#define EXIT_FAILURE func_EXIT_FAILURE(__FILE__,__LINE__);
#endif

void assert_text(std::string cad,int line);

#endif
