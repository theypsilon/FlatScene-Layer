#include "IDebug.h"
#include "tinyxml/tinyxml.h"

int func_fracaso(const char* file, int line) {

	std::string d = "";

	for (int i = strlen(file)-1;i> 0; i--) {
		if (file[i] == '/' ||file[i]== '\\')
			break;
		d = file[i] + d;
	}

	printf("FRACASO en %s line: %d\n",d.c_str(),line);

#ifdef AVOID_FRACASO
	exit(0);
#endif

	return -1;
}

void assert_xml(int code,int line) {
	if (code == TIXML_SUCCESS)
		return;

	std::string cad = "El atributo no existe. Line ";

	if ( code == TIXML_WRONG_TYPE )
		cad = "El atributo es de tipo inv�lido. Line ";

	cad += line  ;

	FSLib.Error("XML Error : "+cad,TE_controlViolation);
}

void assert_text(std::string cad,int line) {
	if (cad.empty() || cad == "")
		FSLib.Error("XML Error : "+line,TE_controlViolation);
}
