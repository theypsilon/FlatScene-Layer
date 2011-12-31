#include "FSMessageHandler.h"
#include "FSLibrary.h"

//inicialización de miembros estáticos
Uint32 FSMessageHandler::s_NextMSGID=1;

//asignación de identificador a mensajes.
Uint32 FSMessageHandler::MSGID_addChild=FSMessageHandler::getNextMSGID();
Uint32 FSMessageHandler::MSGID_RemoveChild=FSMessageHandler::getNextMSGID();
#ifdef MENSAJES_MSGIDS
int FSMessageHandler::textM0=-1;
#endif


FSMessageHandler::FSMessageHandler(FSMessageHandler * pmhParent):
m_pmhParent(NULL) {

	setParent(pmhParent);
}


FSMessageHandler::~FSMessageHandler() {

	setParent(NULL);
}


void FSMessageHandler::setParent(FSMessageHandler * pmhNewParent) {

	if(HasParent())
		SendMessage(MSGID_RemoveChild,(MSGPARM)getParent(),(MSGPARM)this);

	m_pmhParent=pmhNewParent;


	if(HasParent())
		SendMessage(MSGID_addChild,(MSGPARM)getParent(),(MSGPARM)this);

}

FSMessageHandler * FSMessageHandler::getParent() {

	return(m_pmhParent);

}


bool FSMessageHandler::HasParent() {

	return(getParent()!=NULL);

}


int FSMessageHandler::SendMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

#ifdef MENSAJES_MSGIDS
	int parametros=0;
	if (Parm1)
		parametros++;
	if (Parm2)
		parametros++;
	if (textM0>=0)
		Write.erase(textM0);
	textM0 = Write.line(0,240,5,"Last MSG [%d] %dP",(int)MsgID,parametros);
#endif

	if ((MsgID % 2) == 0)		
		if(onMessage(MsgID,Parm1,Parm2) == EXITO) 
			return EXITO;
	
		else 
			if(HasParent())
				return(getParent()->SendMessage(MsgID,Parm1,Parm2));
			else 
				return FRACASO;
	else
		bufferMessages.push_back(new FSMessage(MsgID,Parm1,Parm2));

	return EXITO;
	
}

int FSMessageHandler::onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {
	if(MsgID==MSGID_addChild) {
		if(this==(FSMessageHandler*)Parm1) {
			OnaddChild((FSMessageHandler*)Parm2);
			return EXITO;
		} else {

			return FRACASO;
		}

	} else if(MsgID==MSGID_RemoveChild) {
		if(this==(FSMessageHandler*)Parm1) {
			onRemoveChild((FSMessageHandler*)Parm2);
			return EXITO;
		} else {

			return FRACASO;
		}
		
	}	else  {
		FSLibrary::Error("Mensaje de tipo no registrado",TE_standard);
		return FRACASO;
	}
	
}

void FSMessageHandler::readMessages() {
	MessageCollection::iterator it ;
	FSMessage* m;
	while ( !bufferMessages.empty ( ) )
	{
		it = bufferMessages.begin ( ) ;
		m = *it;
		pendingMessage(m->getMsgID(),m->getParm(1),m->getParm(2));
		bufferMessages.erase ( it ) ;
		delete m;
	}
}

void FSMessageHandler::clearAllMessages() {
	MessageCollection::iterator it ;
	FSMessage* m;
	while ( !bufferMessages.empty ( ) )
	{
		it = bufferMessages.begin ( ) ;
		m = *it;
		bufferMessages.erase ( it ) ;
		delete m;
	}
}

void FSMessageHandler::pendingMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2) {
}

Uint32 FSMessageHandler::getNextMSGID(bool instant) {

	// Si instant, el MSGID debe ser par, si no instant, inpar.

	if (s_NextMSGID%2==0)
		if (instant)
			s_NextMSGID+=2;
		else
			s_NextMSGID++;
	else
		if (instant)
			s_NextMSGID++;
		else
			s_NextMSGID+=2;

	return(s_NextMSGID);
}

void FSMessageHandler::OnaddChild(FSMessageHandler* pmhChild) {

}

void FSMessageHandler::onRemoveChild(FSMessageHandler* pmhChild) {
	
}

FSMessageHandler::FSMessage::FSMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2) 
:	MsgID(MsgID) , Parm1(Parm1), Parm2(Parm2) {
}

FSMessageHandler::FSMessage::~FSMessage() {
}

Uint32 FSMessageHandler::FSMessage::getMsgID() {
	return MsgID;
}

MSGPARM FSMessageHandler::FSMessage::getParm(int n) {
	if (n==1) {
		return Parm1;
	} else if (n==2) {
		return Parm2;
	} else {
		FSLibrary::Error("Parámetro requerido imposible.");
		return (void*)FRACASO;
	}
}
