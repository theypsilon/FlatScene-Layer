#include "FSMessageHandler.h"
#include "FSLibrary.h"

//inicialización de miembros estáticos
Uint32 CMessageHandler::s_NextMSGID=1;

//asignación de identificador a mensajes.
Uint32 CMessageHandler::MSGID_addChild=CMessageHandler::getNextMSGID();
Uint32 CMessageHandler::MSGID_RemoveChild=CMessageHandler::getNextMSGID();
#ifdef MENSAJES_MSGIDS
int CMessageHandler::textM0=-1;
#endif


CMessageHandler::CMessageHandler(CMessageHandler * pmhParent):
m_pmhParent(NULL) {

	setParent(pmhParent);
}


CMessageHandler::~CMessageHandler() {

	setParent(NULL);
}


void CMessageHandler::setParent(CMessageHandler * pmhNewParent) {

	if(HasParent())
		SendMessage(MSGID_RemoveChild,(MSGPARM)getParent(),(MSGPARM)this);

	m_pmhParent=pmhNewParent;


	if(HasParent())
		SendMessage(MSGID_addChild,(MSGPARM)getParent(),(MSGPARM)this);

}

CMessageHandler * CMessageHandler::getParent() {

	return(m_pmhParent);

}


bool CMessageHandler::HasParent() {

	return(getParent()!=NULL);

}


int CMessageHandler::SendMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {

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
		bufferMessages.push_back(new CMessage(MsgID,Parm1,Parm2));

	return EXITO;
	
}

int CMessageHandler::onMessage(Uint32 MsgID,MSGPARM Parm1,MSGPARM Parm2) {
	if(MsgID==MSGID_addChild) {
		if(this==(CMessageHandler*)Parm1) {
			OnaddChild((CMessageHandler*)Parm2);
			return EXITO;
		} else {

			return FRACASO;
		}

	} else if(MsgID==MSGID_RemoveChild) {
		if(this==(CMessageHandler*)Parm1) {
			onRemoveChild((CMessageHandler*)Parm2);
			return EXITO;
		} else {

			return FRACASO;
		}
		
	}	else  {
		CLibrary::Error("Mensaje de tipo no registrado",TE_standard);
		return FRACASO;
	}
	
}

void CMessageHandler::readMessages() {
	MessageCollection::iterator it ;
	CMessage* m;
	while ( !bufferMessages.empty ( ) )
	{
		it = bufferMessages.begin ( ) ;
		m = *it;
		pendingMessage(m->getMsgID(),m->getParm(1),m->getParm(2));
		bufferMessages.erase ( it ) ;
		delete m;
	}
}

void CMessageHandler::clearAllMessages() {
	MessageCollection::iterator it ;
	CMessage* m;
	while ( !bufferMessages.empty ( ) )
	{
		it = bufferMessages.begin ( ) ;
		m = *it;
		bufferMessages.erase ( it ) ;
		delete m;
	}
}

void CMessageHandler::pendingMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2) {
}

Uint32 CMessageHandler::getNextMSGID(bool instant) {

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

void CMessageHandler::OnaddChild(CMessageHandler* pmhChild) {

}

void CMessageHandler::onRemoveChild(CMessageHandler* pmhChild) {
	
}

CMessage::CMessage(Uint32 MsgID, MSGPARM Parm1, MSGPARM Parm2) 
:	MsgID(MsgID) , Parm1(Parm1), Parm2(Parm2) {
}

CMessage::~CMessage() {
}

Uint32 CMessage::getMsgID() {
	return MsgID;
}

MSGPARM CMessage::getParm(int n) {
	if (n==1) {
		return Parm1;
	} else if (n==2) {
		return Parm2;
	} else {
		CLibrary::Error("Parámetro requerido imposible.");
		return (void*)FRACASO;
	}
}
