#include "request.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string.h"
#include "debug.h"


static struct request req;

static  struct fd_element * _nodes;


int request_data_parse(char *buf){
    
    INFO("in parse_request...");
    
    //-----parse data------
    int strCnt =0;
    
    req.srcName= &buf[0];
    
    strCnt = strlen(req.srcName)+1;
    req.destName = &buf[strCnt];
    
    strCnt += strlen(req.destName)+1;
    req.data = &buf[strCnt];
    
    strCnt += strlen(req.data)+1;
    
    return strCnt;
}

//-----------input name string to hash talbe------------
void request_save_srcfd_to_hash(int src_fd){
    
    struct fd_element *p_elem = NULL;
    HASH_FIND_INT(_nodes,&req.srcName,p_elem);
    
    if (!p_elem){
        
        
        HASH_ADD_INT(_nodes,fd,elem);
    }

}

int request_get_destfd_from_hash(void){
    
    struct fd_element *p_elem = NULL;
    
    HASH_FIND_INT(_nodes,&req.destName,p_elem);
    
    if(elem)
    {
        return p_elem->fd;
    }
    
    return -1;
}

int request_remove_fd(char *name){
    
    struct fd_element *p_elem = NULL;
    
    HASH_FIND_INT(_nodes,&name,p_elem);
    
    if(elem)
    {
        HASH_DEL(_nodes,p_elem);
        return 0;
    }
    
    return -1;
    
}





