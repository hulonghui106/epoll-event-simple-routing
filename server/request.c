

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "request.h"

#include "uthash.h"

//--------------------------data type-------------------------
struct request{
    char *srcName;
    char *destName;
    char *data;
};

struct request_fd_element
{
    char *name;
    int fd;
    
    UT_hash_handle hh;
};

typedef struct request_fd_element request_fd_element_t;

#define request_fd_element_s sizeof(request_fd_element_t)


//---------globe var------------------------------------------
static struct request req;
static request_fd_element_t * _nodes;



//---------private API----------------------------------------

static request_fd_element_t * request_fd_new(int fd)
{
    INFO("request_fd_new");
    
    request_fd_element_t *elem = calloc(1, request_fd_element_s);
    
    if (elem)
    {
        elem->fd = fd;
        elem->name = calloc(1, strlen(req->req.srcName)));
        strcpy(elem->name,req->req.srcName,strlen(req->req.srcName));
    }
    return elem;
}


static void request_fd_delete(request_fd_element_t * elem)
{
    INFO("request_fd_delete");
    
    free(elem->name);
    free(elem);
}


//--------------public API----------------------------------------
int request_data_parse(char *buf){
    
    INFO("request_data_parse");
    
    int strCnt =0;
    
    req.srcName= &buf[0];
    
    strCnt = strlen(req.srcName);
    req.destName = &buf[strCnt];
    
    strCnt += strlen(req.destName);
    req.data = &buf[strCnt];
    
    strCnt += strlen(req.data);
    
    return strCnt;
}


void request_save_srcfd_to_hash(poll_event_element_t *node){
    
    INFO("request_save_srcfd_to_hash");
    
    struct fd_element *p_elem = NULL;
    HASH_FIND_STR(_nodes,req.srcName,p_elem);
    
    if (!p_elem){
        
        p_elem = request_fd_new(node->fd);
        node->data = p_elem->name;
        //HASH_ADD_STR(_nodes,req.srcName,p_elem);
        HASH_ADD_KEYPTR( hh, _nodes, p_elem->name, strlen(p_elem->name), p_elem );
    }
}

int request_get_destfd_from_hash(void){
    
    INFO("request_get_destfd_from_hash");
    
    struct fd_element *p_elem = NULL;
    
    HASH_FIND_STR(_nodes,req.destName,p_elem);
    
    if(p_elem)
    {
        return p_elem->fd;
    }
    
    return -1;
}

int request_remove_fd(char *name){
    
    INFO("request_remove_fd");
    
    struct fd_element *p_elem = NULL;
    
    HASH_FIND_STR(_nodes,name,p_elem);
    
    if(elem)
    {
        
        HASH_DEL(_nodes,p_elem);
        request_fd_delete(p_elem);
        
        return 0;
    }
    
    return -1;
    
}





