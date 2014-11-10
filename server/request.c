

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"

#include "request.h"
#include "uthash.h"

#define name_str_Size 12

struct request_fd_element
{
    char name[name_str_Size];
    int fd;
    
    UT_hash_handle hh;
};

typedef struct request_fd_element request_fd_element_t;
#define request_fd_element_s sizeof(request_fd_element_t)
static request_fd_element_t * _nodes;


static  char msg_srcName[name_str_Size];
static  char msg_destName[name_str_Size];
static  char *msg_data;

//---------private API----------------------------------------

static request_fd_element_t * request_fd_new(int fd)
{
    request_fd_element_t *elem = calloc(1, request_fd_element_s);
    
    if (elem)
    {
        elem->fd = fd;
        strcpy(elem->name,msg_srcName);
    }
    
    LOG("request_fd_new:%s,%d\n",elem->name,elem->fd);
    return elem;
}


static void request_fd_delete(request_fd_element_t * elem)
{
    LOG("request_fd_delete:%s,%d\n",elem->name,elem->fd);
    
    free(elem);
}


//--------------public API----------------------------------------
int request_data_parse(char *buf){
    
    INFO("request_data_parse\n");
    
    //LOG("%s\n",buf);
    if (strlen(buf)<22) {
        return -1;
    }
    
    strncpy(msg_srcName,buf,10);
    msg_srcName[10]='\0';
    
    strncpy(msg_destName,&buf[10],10);
    msg_destName[10]='\0';

    msg_data = &buf[20];
    
    int strCnt = strlen(msg_data)+21;
    
    LOG("%s\n",msg_srcName);
    LOG("%s\n",msg_destName);
    LOG("%s\n",msg_data);
    LOG("%d\n",strCnt);
  
    return strCnt;
}

extern int event_manager_remove_element(int fd);

void request_save_srcfd_to_hash(int socket_fd,void **data){
    
    request_fd_element_t *p_elem = NULL;
    HASH_FIND_STR(_nodes,msg_srcName,p_elem);
    
    if (!p_elem){
        
        p_elem = request_fd_new(socket_fd);
        *data = p_elem->name;
        HASH_ADD_STR(_nodes,name,p_elem);
        
        LOG("request_save_srcfd_to_hash:%s,%d\n",*data,socket_fd);
    }
    else{
        LOG("request_save_srcfd_to_hash:%s,%d !!already\n",msg_srcName,socket_fd);
        if(socket_fd != p_elem->fd){
            LOG("request_save_srcfd_to_hash:%d,%d !!fd change(one client)\n",p_elem->fd,socket_fd);
            event_manager_remove_element(p_elem->fd);
            p_elem->fd=socket_fd;
        }
    }
    
    
}

int request_get_destfd_from_hash(void){
    
    request_fd_element_t *p_elem = NULL;
    
    HASH_FIND_STR(_nodes,msg_destName,p_elem);
    
    if(p_elem)
    {
        LOG("request_get_destfd_from_hash:%s,%d\n",msg_destName,p_elem->fd);
        return p_elem->fd;
    }
    else{
        LOG("request_get_destfd_from_hash:%s,!!not found\n",msg_destName);
    }
    
    return -1;
}

int request_remove_fd(char *data){
    
    if (data == NULL) {
        return 0;
    }
    
    request_fd_element_t *p_elem = NULL;
    
    HASH_FIND_STR(_nodes,data,p_elem);
    
    if(p_elem)
    {
        LOG("request_remove_fd:%s,%d\n",data,p_elem->fd);
        HASH_DEL(_nodes,p_elem);
        request_fd_delete(p_elem);
    }
    else{
        LOG("request_remove_fd:%s !!!hash can't find\n",data);
    }
    
    return 0;
    
}





