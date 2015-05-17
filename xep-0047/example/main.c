#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include <strophe.h>
#include "common.h"

#include "xmppclient.h"
#include "xmpp_ibb.h"

xmpp_conn_t*  XMPP_Init(char* jid, char* pass, char* host, xmpp_ctx_t  **pctx);
void XMPP_Close(xmpp_ctx_t *ctx,  xmpp_conn_t *conn);

pthread_attr_t attr1;
pthread_t pid1;

int  main(int argc, char **argv)
{
 
    xmpp_conn_t* conn = NULL;
    xmpp_ctx_t *ctx = NULL;    
    char *jid, *pass, *host;	
    int errornum=0;
    void *ptr;
    pthread_attr_init(&attr1);
    pthread_attr_setstacksize(&attr1, PTHREAD_STACK_MIN * 2);

      
    /* take a jid and password on the command line */
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: xep0047-test  <jid> <pass> [<host>]\n\n");
        return 0;
    }

    jid = argv[1];
    pass = argv[2];
    host = NULL;

    if (argc == 4)
        host = argv[3];

    conn = XMPP_Init(jid , pass, host, &ctx);

    if(ctx !=NULL)
        pthread_create(&pid1, \
        &attr1, (void*)xmpp_run, (void*)ctx );

    
    while(1)
    {
        sleep(1);
        
        if(XMPP_IBB_Get_Recv() != NULL)
	{
            XMPP_IBB_SendPayload(conn, XMPP_IBB_Get_gStanza(), \
   (void*)ctx, XMPP_IBB_Get_Recv() );//Echo test            
            
            XMPP_IBB_Reset_Recv(); 
            XMPP_IBB_Reset_gStanza(); 

        }

    }
  
    xmpp_stop(ctx);
    sleep(1);
    XMPP_Close(ctx, conn);


    if ((errornum = pthread_cancel(pid1)) != 0)
        fprintf(stderr, "pthread_cancel: %s", strerror(errornum));
    if ((errornum = pthread_join(pid1, &ptr)) != 0)
        fprintf(stderr, "pthread_join: %s", strerror(errornum));

    return 0;
 
}

