#include "Animation.h"
#include "Library.h"
#include "ControlImages.h"

CAnimation::CAnimation() : seq(0), paso(0) {
}

CAnimation::~CAnimation() {
	if (sequence && limitofsequence && sptset && flags) {
		for (int i=0;i<num;i++) {
			freemem(sequence[i]);
			CImg.remove(sptset[i]);
		}
		freemem(sequence);
		freemem(flags);
		freemem(limitofsequence);
		freemem(sptset);
		flags=NULL;
		sequence=NULL;
		limitofsequence=NULL;
		sptset=NULL;
	}
}

void CAnimation::cloneThisInto(CAnimation* clon) {

		if (sequence && limitofsequence && sptset && flags) {
			clon->num = num;
			clon->limitofsequence= alloc(int,num);
			clon->flags= alloc(int,num);
			clon->sptset= alloc(int,num);
			clon->sequence= alloc(int*,num);
			for (int i=0;i<num;i++) {
				clon->limitofsequence[i] = limitofsequence[i];
				clon->flags[i] = flags[i];
				clon->sptset[i] = CImg.add(CImg.get(sptset[i])->getName().c_str());
				clon->sequence[i] = alloc(int,limitofsequence[i]);
				for (int j=0;j<limitofsequence[i];j++)
					clon->sequence[i][j]=sequence[i][j];
			}
		} else 
			CLibrary::Error("Se pretende clonar una acción que aún no se ha definido.");

}

int CAnimation::nextStep() {

	int spt = sequence[seq][paso>>3];

	paso++;
	if ((paso>>3) >= limitofsequence[seq])
		paso=0;

	return spt;
}

void CAnimation::reinicia() {
	seq=0;
	paso=0;
}

void CAnimation::generateAnimationByXML(TiXmlElement* newNode, const char* owner) {

	TiXmlElement* nodeSequence = newNode->FirstChildElement("animation");	// Buscamos la animación.

	if (nodeSequence)	// Si existe, buscamos la primera secuencia de la animación.
		nodeSequence = nodeSequence->FirstChildElement("sequence");

	if (!nodeSequence)	// Si no, falla.
		CLibrary::Error("Animación no encontrada en el fichero de acciones.");

	list<string> files;
	list<int*> sequences;
	list<int> limiteSeq;
	list<int> flags;

	while (nodeSequence && nodeSequence->Attribute("fileall")) {
		string file = nodeSequence->Attribute("fileall");

		if(strcmp(file.substr(0,1).c_str(),"*")==0) {	// Si es un alias...
			aliasFileAnimation(file,owner);	// Busca el archivo apropiado y lo devuelve a file.
		} // Una vez con el nombre del fichero de gráficos en file... 

		if (nodeSequence && nodeSequence->Attribute("flags")) { // Si se indican las flags...
			flags.push_back(atoi(nodeSequence->Attribute("flags")));
		} else // Si no, por defecto 0.
			flags.push_back(0);

		TiXmlElement* step = nodeSequence->FirstChildElement("step");

		list<int> steps;
		while (step && step->FirstChildElement("img") && step->FirstChildElement("img")->Attribute("value")) { // Mientras no acabe la secuencia...
			steps.push_back(atoi(step->FirstChildElement("img")->Attribute("value")));	// En el futuro, cada paso de la secuencia puede que esté compuesto por más imágenes.
			step = step->NextSiblingElement();
		}

		if (steps.empty())
			CLibrary::Error("Secuencia de animación sin al menos 1 paso en el arbol de acciones.");

		int* seq = alloc(int,steps.size());

		list<int>::iterator it=steps.begin();
		for (int i=0;i<steps.size();i++) {
			seq[i]=(*it);
			++it;
		}
		limiteSeq.push_back(steps.size());
		sequences.push_back(seq);
		files.push_back(file);

		steps.clear();

		nodeSequence = nodeSequence->NextSiblingElement();
	}

	if (sequences.empty() || sequences.size()!=limiteSeq.size() || sequences.size()!=flags.size() || sequences.size()!=files.size()) {
		CLibrary::Error("Conflicto generado en la captura de la animación.");
	}

	this->num=sequences.size();
	this->limitofsequence= alloc(int,this->num);
	this->flags= alloc(int,this->num);
	this->sptset= alloc(int,this->num);
	this->sequence=  alloc(int*,this->num);
	for (int i=0;i<this->num;i++) {
		this->limitofsequence[i]=limiteSeq.front();
		limiteSeq.pop_front();
		this->flags[i] = flags.front();
		flags.pop_front();
		this->sptset[i]=CImg.add(files.front().c_str());
		files.pop_front();
		this->sequence[i] = sequences.front();
		sequences.pop_front();
	}

	if (!limiteSeq.empty() || !flags.empty() || !files.empty() || !sequences.empty())
		CLibrary::Error("El buffer de creación de la animación no ha sido vaciado.");

	flags.clear();
	sequences.clear();
	limiteSeq.clear();
	files.clear();
	
}

void CAnimation::aliasFileAnimation(std::string &file,const char* owner) {
	TiXmlDocument xmldoc(resource("animationalias.xml").c_str());
	if (!xmldoc.LoadFile()) {	 CLibrary::Error(resource("animationalias.xml").c_str(),TE_fileExists); }

	TiXmlHandle input(xmldoc.FirstChild());

	file= file.substr(1,file.size());
	TiXmlElement*	animAlias = input.FirstChildElement("alias").ToElement(); // Buscamos el correspondiente a la actual criatura en el arbol de alias.

	while (animAlias && strcmp(animAlias->Attribute("id"),file.c_str())!=0) 
		animAlias = animAlias->NextSiblingElement();

	if (animAlias) {
		animAlias = animAlias->FirstChildElement("animation");	// Buscamos la animación cuyo owner coincida con este caso.
		while (animAlias && strcmp(animAlias->Attribute("owner"),owner)!=0)
			animAlias = animAlias ->NextSiblingElement();

		if (animAlias)
			file = animAlias->Attribute("name");
	}

	if (!animAlias)
		CLibrary::Error("El alias reclamado no se corresponde con ninguna animación registrada.");
}
