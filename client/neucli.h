

#ifndef __NEUCLI_H__
#define __NEUCLI_H__

#include "fixarray.h"

#include <event2/listener.h>
#include <sys/socket.h>

#ifdef __cpluscplus
extern "C" {
#endif

typedef int (*neucli_on_connect)(neucli_t *cli, int status, int errcode);
typedef int (*neucli_on_data)(neucli_t *cli);

// neucli_on_connect status
enum {
    NEUCLI_CONN_INIT = 0,
    NEUCLI_CONN_CONNECTED,
    NEUCLI_CONN_SYNCED,
};

typedef struct neucli {
    int efd;

    int state;

    neucli_on_connect onconn;
    neucli_on_data    ondata;

    neuevent_t nev;
    fixmap_t   fmslots;
}neucli_t;

int neucli_init(neucli_t *cli, neucli_on_connect onconn, neucli_on_data ondata);
int neucli_fini(neucli_t *cli);
int neucli_connect(neucli_t *cli, struct sockaddr *addr, int len);
int neucli_status(neucli_t *cli);
int neucli_send(neucli_t *cli, neupdu_t *pdu);

/** message

struct example{};
struct example_rsp{};

// generate skeleton for user
extern void example_cb_impl(void *data, uint32_t seq, struct example_rsp *rsp);

static void example_rsp(void *data, uint32_t seq, struct neupdu *rsp){
    struct example_rsp *rsp = example_rsp_unmashal(rsp);
    
    example_cb_impl(data, seq, rsp);
    
    example_rsp_free(rsp);
}

int send_example(struct neusrv *srv, struct example *ex, void *data, uint32_t *seq){
    struct neupdu *req = example_mashal(ex);
    int ret;

    ret = send_example(srv, req, example_rsp, data, seq);
    neupdu_free(req);

    return ret;
} 
*/

typedef void (*neucli_msg_rspcb)(void *data, uint32_t seq, neupdu_t *rsp);

typedef struct neucli_msg_reqrsp{
    fixmap_node_t  node;

    neucli_msg_rspcb  cb;
    void *data;
    uint32_t seq;
}neucli_msg_reqrsp_t;

typedef struct neucli_msg{
    neucli_t *cli;

    fixmap_t fmmsgs;
}neucli_msg_t;

int neucli_msg_setup(neucli_t *cli, const char *msg, struct neucli_msg **mcli);
int neucli_msg_close(struct neucli_msg *mcli);

// message name send()
int neucli_msg_send(struct neucli_msg *mcli, struct neupdu *req,
                        neucli_msg_rspcb cb, void *data, uint32_t *seq);

/** message queue
*/
/*
int neusrv_setup_msq(struct neusrv *srv, const char *msq, struct meumsqcli *mcli);
int neusrv_close_msq(struct neumsqcli *mcli);

typedef void (*neumsq_cb)(struct neumsqcli *mcli, void *data, struct neupdu *msg);
int neusrc_subscribe(struct neumsqcli *mcli, const char *msq, neumsq_cb cb, void *data); 
int neusrc_unsubscribe(struct neumsqcli *mcli);
*/

/** RPC
*/
/*
int neusrv_setup_rpc(struct neusrv *srv, const char *service, struct neurpc **rpc);
int neusrv_close_rpc(struct neurpc *rpc);
*/
/*
// sync mode
struct outs* neurpc_example(struct neurpc *rpc, struct example *e, struct sample *s){
    struct __example_param *ep = {e, s};
    struct neupdu *req = __example_mashal(ep);
    struct neupdu *rsp = NULL;
    struct outs *o;
    int ret;

    // func -- generate by compiler
    ret = neurpc_call(rpc, func, req, &rsp);
    neupdu_free(req);
    if(ret != 0){
        return NULL;
    }

    o = __example_outs_unmashal(rsp); 
    neupdu_free(rsp);
    return o;
}

// async mode
typedef void (*neurpc_examplecb)(void *data, uint32_t seq, struct outs *o);
void example_cb(void *data, int seq, struct outs *o){
    //
    // user implememt code here
    //
}

static void neurpc_example_callback(struct neurpc *rpc, uint32_t seq, void *data,
                    struct neupdu *rsp){
    struct __example_cbdata *ec = (struct __example_cbdata*)data;
    struct outs *o = __outs_unmashal(rsp);

    ec->cb(ec->data, seq, o);
}

uint32_t neurpc_example(struct neurpc *rpc, struct example *e, struct sample *s,
        neurpc_examplecb cb, void *data){
    struct __example_param ep = {e, s};
    struct __example_cbdata ec = {example_cb, data};
    struct neupdu *req = __example_mashal(&ep);
    uint32_t ret;

    ret = neurpc_call_async(rpc, func, req, neurpc_example_callback, &ec);
    neupdu_free(req);

    return ret;
}

*/
/*
typedef void (*neurpc_cb)(struct neurpc *rpc, unint32_t seq, void *data, struct neupdu *rsp);
int neurpc_call(struct neurpc *rpc, int func, struct neupdu *req, struct neupdu **rsp);
uint32_t neurpc_call_async(struct neurpc *rpc, int func, struct neupdu *req, 
            neurpc_cb cb, void *data);
*/
#ifdef __cpluscplus
}
#endif

#endif // __NEUCLI_H__

