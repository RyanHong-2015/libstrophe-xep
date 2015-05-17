#include "strophe.h"

#define XMLNS_IBB "http://jabber.org/protocol/ibb"

typedef struct _xmpp_ibb_data_t {
	char* seq_num;
	char* recv_data;	
	struct _xmpp_ibb_data* next;

} xmpp_ibb_data_t;

typedef struct _xmpp_ibb_session_t {

	xmpp_conn_t* conn;
	char* sid;
	char* szblock_size;
	xmpp_ibb_data_t* ibb_data_queue;
	struct _xmpp_ibb_session_t* next;		

} xmpp_ibb_session_t;  


xmpp_ibb_session_t* XMPP_Get_IBB_Session_Handle(char* szSid);

int XMPP_IBB_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata);

int XMPP_IBB_data_process(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata);

void XMPP_IBB_SendPayload(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza,
void * const userdata, char* resp);

void XMPP_IBB_Data_Send(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata);

void XMPP_IBB_Ack_Send(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata);

void XMPP_IBB_Close_Send(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata);


char* XMPP_IBB_Get_Recv();
void XMPP_IBB_Reset_Recv();
xmpp_stanza_t* XMPP_IBB_Get_gStanza();
void XMPP_IBB_Reset_gStanza();


