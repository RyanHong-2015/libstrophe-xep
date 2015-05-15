#include <stdio.h>
#include <stdlib.h>
#include <strophe.h>
#include "common.h"

#define MAX_XMPPUN_LEN 128
#define MAX_XMPPPW_LEN 128
#define MAX_XMPPSV_LEN 128


//char xmpp_un[MAX_XMPPUN_LEN]="ryanh\\40workssys.com@cloud01.workssys.com/Ryan_903";
char xmpp_un[MAX_XMPPUN_LEN]="wks.rds\\40gmail.com@cloud01.workssys.com/Ryan_RAE";
//char xmpp_pw[MAX_XMPPPW_LEN]="85jlufvf3mmebvebt65cb9nc8s";
char xmpp_pw[MAX_XMPPPW_LEN]="a0idvpmmvc6jnpb6aji6bgslre";
char xmpp_server[MAX_XMPPSV_LEN]="cloud01.workssys.com";

xmpp_conn_t*  XMPP_Init(char* jid, char* pass, char* host, xmpp_ctx_t  **pctx);
void XMPP_Close(xmpp_ctx_t *ctx,  xmpp_conn_t *conn);


#if 0
/* define a handler for connection events */
void conn_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status,
                  const int error, xmpp_stream_error_t * const stream_error,
                  void * const userdata)
{



}
#endif

void main()
{

#if 1
    xmpp_conn_t* conn = NULL;
    xmpp_ctx_t *ctx = NULL;
    conn = XMPP_Init(xmpp_un , xmpp_pw, xmpp_server, &ctx);
    


//    Register_IBB_CB(conn, xmpp_server, 0 );


    xmpp_run(ctx);

    xmpp_stop(ctx);
    sleep(1);
    XMPP_Close(ctx, conn);
#endif

}
#if 0 
xmpp_conn_t*  XMPP_Init(char* jid, char* pass, char* host, xmpp_ctx_t  **pctx)
{
    xmpp_conn_t *conn;
    xmpp_log_t *log;

    printf("jid=[%s] pass=[%s] host=[%s]", jid, pass, host);
    xmpp_initialize();

    /* create a context */
    log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG); /* pass NULL instead to silence output */
    *pctx = xmpp_ctx_new(NULL, log);

    /* create a connection */
    conn = xmpp_conn_new(*pctx);
    /* setup authentication information */
    xmpp_conn_set_jid(conn, jid);
    xmpp_conn_set_pass(conn, pass);

    /* initiate connection */
    xmpp_connect_client(conn, host, 0, conn_handler, *pctx);
    return conn;
}


void XMPP_Close(xmpp_ctx_t *ctx,  xmpp_conn_t *conn)
{

    if(conn !=NULL)
        xmpp_disconnect(conn);
  
 //   xmpp_handler_delete(conn, iq_ibb_open_handler);

    xmpp_conn_release(conn);

    fprintf(stderr, "Conn release!");

    /* final shutdown of the library */

   if(ctx!=NULL)
        xmpp_ctx_free(ctx);
   xmpp_shutdown();

}


  





#endif