#include "Player.h"
#include "TestAGameInterface.h"
#include "FSLibrary.h"

CPlayer::CPlayer(FSMessageHandler * pmhParent) : CActorScrollMap("PJ0",pmhParent) {
#ifdef MENSAJES_ACCIONES
	textA0=-1;
	textA1=-1;
	textA2=-1;
	textA3=-1;
#endif
	controlThis();
	m_Scrollxy.set(310,100,0);
}

CPlayer::~CPlayer() {
	if (garbage) {
		delete garbage;
		garbage=NULL;
	}
	if (rutinaColision) {
		delete rutinaColision;
		rutinaColision=NULL;
	}
}

void CPlayer::init(list<string>& activationIds, int v) {
#ifdef LOG_JUGADORES
	printf("Iniciando jugador %d...\n",v);
#endif
	//Configuraci�n de teclas del jugador.

	TiXmlDocument xmldoc("resources/config.xml");	// Cargamos el archivo de configuraci�n.
	if (!xmldoc.LoadFile()) {	 FSLib.Error("resources/config.xml",TE_fileExists); }

	TiXmlHandle input(xmldoc.FirstChildElement("Keys"));

	// Primero sustituimos los valores de los botones de cada Player por su correspondiente num�rico que detecta la libreria de I/O.
	for (int k=0, total=atoi(input.FirstChildElement("Alias").ToElement()->Attribute("total"))	;		k<total		;	k++) {
		TiXmlHandle actual = input.FirstChildElement("Alias").ChildElement("keyalias",k);
		TiXmlElement* player = input.FirstChildElement("Player").ToElement();
		while (player) {
			TiXmlElement* buttom = player->FirstChildElement("buttom");
			while (buttom) {
				int c;
				if (c=strcmp(buttom->Attribute("value"),actual.ToElement()->Attribute("id"))==0) 
					buttom->SetAttribute("value",actual.ToElement()->Attribute("value"));
				buttom = buttom->NextSiblingElement();
			}
			player = player->NextSiblingElement();
		}
	}

	// Averiguamos que configuraci�n de jugador es la que pertenece a este.
	TiXmlElement* player = input.FirstChildElement("Player").ToElement();
	while (player) {
		if (atoi(player->Attribute("id"))==v) break;
		player = player->NextSiblingElement();
	}

	if (!player) FSLib.Error("Estructura inv�lida de config.xml");

	// Ahora utilizamos player para navegar por sus botones.
	player = player->FirstChildElement("buttom");
	while (player) {
		configKey((SDLKey)atoi(player->Attribute("value")),player->Attribute("id"));	// Y configuramos las teclas seg�n el archivo de configuraci�n.
		player = player->NextSiblingElement();
	}

	CActorScrollMap::init(activationIds);

}

void CPlayer::configKey(SDLKey sym,const char* keycode) {
	Uint8 key;
	string pk_keycode("PKEY_");
	pk_keycode+=keycode;

	if (pk_keycode=="PKEY_UP") {
		key=PKEY_UP;
	} else if (pk_keycode=="PKEY_DOWN") {
		key=PKEY_DOWN;
	} else if (pk_keycode=="PKEY_LEFT") {
		key=PKEY_LEFT;
	} else if (pk_keycode=="PKEY_RIGHT") {
		key=PKEY_RIGHT;
	} else if (pk_keycode=="PKEY_B1") {
		key=PKEY_B1;
	} else if (pk_keycode=="PKEY_B2") {
		key=PKEY_B2;
	} else if (pk_keycode=="PKEY_B3") {
		key=PKEY_B3;
	} else if (pk_keycode=="PKEY_B4") {
		key=PKEY_B4;
	} else if (pk_keycode=="PKEY_B5") {
		key=PKEY_B5;
	} else if (pk_keycode=="PKEY_B6") {
		key=PKEY_B6;
	} else if (pk_keycode=="PKEY_B7") {
		key=PKEY_B7;
	} else if (pk_keycode=="PKEY_B8") {
		key=PKEY_B8;
	} else{
		FSLib.Error(("Configuraci�n de teclas inv�lidas en config.xml \n"+string(pk_keycode)).c_str());
	}

	KeyAlias[key]=sym;
#ifdef EVENTOS_RAPIDO
	CTestAGameInterface* father=(CTestAGameInterface*) getParent();
	father->updatePlayerKeyAlias(sym,this,key);
#endif
}

int CPlayer::KeyAliasFor(SDLKey sym) {
	for (int i=0;i<8;i++) {
		if (KeyAlias[i]==sym)
			return i;
	}
	return -1;
}		

void CPlayer::blockFutureActionCandidates() {
	CAction* act=getAction();
	if (act) {
		for (int i=0;i<NUM_PJKEY;i++) {
			listActionCandidate.remove(act->getKeyup(i));
			listActionCandidate.remove(act->getKeydown(i));
		}
		CActionMove* actMov = dynamic_cast<CActionMove*> (act);
		if (actMov)	{			
			vector<CAction*>* actionBrothers = actMov->getBrothers();
			if (!actionBrothers || (actionBrothers->size() < 5))
				FSLib.Error("Clase ActionMove falla.");
			setAction(actionBrothers->at(4));
		}
	}
}

int CPlayer::move() {
	int ret = CActorScrollMap::move();

	if (actActual!=actUltimo) {
		if (actUltimo)
			actUltimo->lastExecute();
		actActual-> firstExecute();
		actUltimo = actActual;
	}

	actActual->execute();

#ifdef MENSAJES_ACCIONES
	string s;
	if (actActual) 
		s = actActual->getId();
	else
		s = "NULL";
	if (textA0>=0)
		Write.erase(textA0);
	textA0 = Write.line(0,5,180,"aA=%s ",s.c_str());
	
	if (actUltimo) 
		s = actUltimo->getId();
	else
		s = "NULL";
	if (textA1>=0)
		Write.erase(textA1);
	textA1 = Write.line(0,5,195,"aU=%s ",s.c_str());
	
	if (actNeutro) 
		s = actNeutro->getId();
	else
		s = "NULL";
	if (textA2>=0)
		Write.erase(textA2);
	textA2 = Write.line(0,5,210,"aN=%s ",s.c_str());
	
	if (actUltimoMovimiento) 
		s = actUltimoMovimiento->getId();
	else
		s = "NULL";
	if (textA3>=0)
		Write.erase(textA3);
	textA3 = Write.line(0,5,225,"aUM=%s ",s.c_str());
	
#endif

	return ret;
}

#ifdef MENSAJES_MSGIDS
int CPlayer::SendMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {
	printf("Player 0 :: ");
	return FSMessageHandler::SendMessage(MsgID,Parm1,Parm2);
}
#endif

int CPlayer::onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

	return(FSMessageHandler::onMessage(MsgID,Parm1,Parm2));

}