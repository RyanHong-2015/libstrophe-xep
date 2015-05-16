#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xmppclient.h"
#include "xmpp_ibb.h"

#include <strophe.h>
#include "common.h"

xmpp_ibb_session_t* gXMPP_IBB_handle;


#if 1
int XMPP_IBB_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
    char*  szBlock_size;
    char*  szSid;

    if( xmpp_stanza_get_child_by_name(stanza, "open") != NULL)
    {

        printf(" =====XEP0047 Open Handle\n");
        szBlock_size =  \
            xmpp_stanza_get_attribute(  xmpp_stanza_get_child_by_name(stanza, "open"), "block-size");
        szSid =  \
            xmpp_stanza_get_attribute(xmpp_stanza_get_child_by_name(stanza, "open"), "sid");
    printf("XEP0047 IQ blocksize=%s sid=%s \n",szBlock_size, szSid );

	xmpp_ibb_session_t* ibb_ssn_p;	
	ibb_ssn_p = malloc(sizeof(xmpp_ibb_session_t));
        ibb_ssn_p->sid = malloc(strlen(szSid)+1);		
	ibb_ssn_p->szblock_size = malloc(strlen(szBlock_size)+1);
	ibb_ssn_p->ibb_data_queue = NULL;
        ibb_ssn_p->next = NULL;
	
	strcpy(ibb_ssn_p->sid, szBlock_size);
	strcpy(ibb_ssn_p->szblock_size, szSid);

	//=================> gXMPP_IBB_handle-> next = ibb_ssn_p;
	XMPP_IBB_Add_Session_Queue(ibb_ssn_p);

        XMPP_IBB_Ack_Send(conn, stanza, userdata);

    }
    else  if( xmpp_stanza_get_child_by_name(stanza, "data") != NULL)
    {
        XMPP_IBB_Ack_Send(conn, stanza, userdata);
        printf("========XEP0047 Data process\n");
        XMPP_IBB_Data_Process(conn, stanza  , userdata);

    }
    else if( xmpp_stanza_get_child_by_name(stanza, "close")  )
    {
        XMPP_IBB_Ack_Send(conn, stanza, userdata);
    }
    return 1;
}
#endif

int XMPP_IBB_Data_Process(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{

    char* intext;
    unsigned char *result;
    xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;
    char *szSid, szSeq;	
    xmpp_ibb_session_t* ibb_ssn;
    

    szSid = \
	xmpp_stanza_get_attribute(xmpp_stanza_get_child_by_name(stanza, "data"), "sid");
 
    szSeq = \ 
	xmpp_stanza_get_attribute(xmpp_stanza_get_child_by_name(stanza, "data"), "seq");
 
    intext = xmpp_stanza_get_text(xmpp_stanza_get_child_by_name(stanza, "data"));

    printf("[Sid=%s][Seq=%s][Raw Data=%s]\n", szSid, szSeq, intext);
    result = base64_decode(ctx, intext, strlen(intext));
    printf("Decode result=%s\n", result);
	            
    ibb_ssn = XMPP_Get_IBB_Session_Handle(szSid);

    if( ibb_ssn == NULL)		
    {
        printf("Opened Session ID not found\n"); 
	goto error;
    }

    xmpp_ibb_data_t* ibb_data_new = malloc(sizeof(xmpp_ibb_data_t));
    ibb_data_new->seq_num = malloc(strlen(szSeq)+1);
    ibb_data_new->recv_data =  malloc(strlen(result)+1);
    
    strcpy(ibb_data_new->seq_num, szSeq);   
    strcpy(ibb_data_new->recv_data, result);
   
    XMPP_IBB_Add_Session_Data_Queue(ibb_ssn, ibb_data_new);

error:
    xmpp_free(ctx, szSid);
    xmpp_free(ctx, szSeq);
    xmpp_free(ctx, intext);
    xmpp_free(ctx, result);

    return 1;
}


void XMPP_IBB_SendPayload(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata, char* resp )
{

    static int seq = 0;
    int data_seq = 0;
//    char* buf;
    char Data_Seq_Buf[32];
    char ID_Buf[32];
//    char *resp;
    char* encoded_data;
    xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;
    xmpp_stanza_t *iq, *data,  *text;

    iq  = xmpp_stanza_new(ctx);
    data = xmpp_stanza_new(ctx);
    text = xmpp_stanza_new(ctx);

    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "set");

    sprintf(ID_Buf, "ID-seq-%d", seq);
    seq++;
    xmpp_stanza_set_id(iq, ID_Buf);
    xmpp_stanza_set_attribute(iq, "to", xmpp_stanza_get_attribute(stanza, "from"));
    xmpp_stanza_set_attribute(iq, "from", xmpp_stanza_get_attribute(stanza, "to"));

    xmpp_stanza_set_name(data, "data");

    xmpp_stanza_set_ns(data, XMLNS_IBB);

    xmpp_stanza_set_attribute(data, "sid",   \
    xmpp_stanza_get_attribute(xmpp_stanza_get_child_by_name(stanza, "data"), "sid"));
  
    sprintf(Data_Seq_Buf , "%d", data_seq);
    xmpp_stanza_set_attribute(data, "seq", Data_Seq_Buf);
       
    printf("\n[Response =%s]\n", resp);
    encoded_data = base64_encode(ctx, (unsigned char*)resp, strlen(resp));

    xmpp_stanza_set_text_with_size(text, encoded_data, strlen(encoded_data));
    xmpp_stanza_add_child(data, text);
    xmpp_stanza_add_child(iq, data);
    xmpp_send(conn, iq);
    seq++;

    free(resp);
    xmpp_free(ctx, encoded_data);

    xmpp_stanza_release(data);
    xmpp_stanza_release(iq);
    xmpp_stanza_release(text);

    xmpp_stanza_release(stanza);  //copied by IBB IQ receiver handler

}



void XMPP_IBB_Ack_Send(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
    xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;
    xmpp_stanza_t *iq;

    iq  = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "result");

    xmpp_stanza_set_id(iq, xmpp_stanza_get_attribute(stanza, "id"));
    xmpp_stanza_set_attribute(iq, "to", xmpp_stanza_get_attribute(stanza, "from"));
    xmpp_stanza_set_attribute(iq, "from", xmpp_stanza_get_attribute(stanza, "to"));
    xmpp_send(conn, iq);
    xmpp_stanza_release(iq);
}

#if 1
/* XMPP_IBB_Close_Send() has not been verified. */

void XMPP_IBB_Close_Send(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
   
#if 0
    xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;
    xmpp_stanza_t *iq;

    iq  = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "set");

    xmpp_stanza_set_id(iq, xmpp_stanza_get_attribute(stanza, "id"));
    xmpp_stanza_set_attribute(iq, "to", xmpp_stanza_get_attribute(stanza, "from"));
    xmpp_stanza_set_attribute(iq, "from", xmpp_stanza_get_attribute(stanza, "to"));

    xmpp_send(conn, iq);
    xmpp_stanza_release(iq);

#endif

}
#endif

xmpp_ibb_session_t* XMPP_IBB_Init()
{
  
    gXMPP_IBB_handle = malloc(sizeof(xmpp_ibb_session_t));

    gXMPP_IBB_handle->conn = NULL;
    gXMPP_IBB_handle->sid = NULL;
    gXMPP_IBB_handle->szblock_size = NULL;
    gXMPP_IBB_handle->ibb_data_queue = NULL;
    gXMPP_IBB_handle->next = NULL;
	
    return gXMPP_IBB_handle;	
}
xmpp_ibb_session_t* XMPP_Get_IBB_Handle()
{
    return gXMPP_IBB_handle;
}

xmpp_ibb_session_t* XMPP_Get_IBB_Session_Handle(char* szSid)
{
    xmpp_ibb_session_t* ibb_ssn_p  = XMPP_Get_IBB_Handle();
    
    while(ibb_ssn_p!=NULL)
    {	
        if(strcmp(ibb_ssn_p->sid, szSid) == 0 )
	    return ibb_ssn_p;	
	else
 	    ibb_ssn_p= ibb_ssn_p->next; 
    }
   	    
    return NULL;

}

void  XMPP_IBB_Add_Session_Queue(xmpp_ibb_session_t* ibb_ssn_new)
{
    xmpp_ibb_session_t* ibb_ssn_p  = XMPP_Get_IBB_Handle();

    while(ibb_ssn_p!=NULL)
    {
        if( ibb_ssn_p->next == NULL )
            ibb_ssn_p->next = ibb_ssn_new;
        else
            ibb_ssn_p = ibb_ssn_p->next;
    }
   
}

void XMPP_IBB_Add_Session_Data_Queue(xmpp_ibb_session_t* ibb_ssn, \
xmpp_ibb_data_t*  ibb_data_new)
{
 
    xmpp_ibb_data_t* ibb_data_p;
    ibb_data_p = ibb_ssn->ibb_data_queue;

    while( ibb_data_p!=NULL )
    {
        if( ibb_data_p->next == NULL )
            ibb_data_p->next = ibb_data_new;
        else
             ibb_data_p = ibb_data_p->next;
    }

}



