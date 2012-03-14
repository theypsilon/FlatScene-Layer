#ifndef _I_DEBUG_
#define _I_DEBUG_

#include "FSLibrary.h"

using namespace flatscene;

int func_fracaso(const char* file, int line);

#define AVOID_FRACASO

#ifdef AVOID_FRACASO
	#undef FRACASO
	#define FRACASO func_fracaso(__FILE__,__LINE__);
#endif

#define idebug_xml

void assert_xml(int code,int line);

void assert_text(std::string cad,int line);

#ifdef idebug_xml
	#define axml(tinyxmlcall) assert_xml(tinyxmlcall, __LINE__)
#else
	#define axml(tinyxmlcall) tinyxmlcall
#endif

#ifdef idebug_xml
	#define atxml(tinyxmlcall) assert_text(tinyxmlcall, __LINE__)
#else
	#define atxml(tinyxmlcall) tinyxmlcall
#endif

#endif