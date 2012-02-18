#include "IDebug.h"

int func_fracaso(const char* file, int line) {

	string d = "";

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

	string cad = "El atributo no existe. Line ";

	if ( code == TIXML_WRONG_TYPE )
		cad = "El atributo es de tipo inválido. Line ";

	cad += line  ;

	CLibrary::Error("XML Error : "+cad,TE_controlViolation);
}

void assert_text(string cad,int line) {
	if (cad.empty() || cad == "")
		CLibrary::Error("XML Error : "+line,TE_controlViolation);
}
