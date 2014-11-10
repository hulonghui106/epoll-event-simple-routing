
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "event_manager.h"
#include "server.h"

#include "uthash.h"


//-------------------------------event_element---------------------------
struct event_element_st
{
    /** the file descriptor*/
    int fd;
    
    /** user data for this element*/
    void * data;
    /** epoll events flags */
    uint32_t events;
    
    /** only used to enable accept and listen callbacks */
    uint8_t cb_flags;
    
    UT_hash_handle hh;
};

typedef struct event_element_st event_element_t;
#define event_element_s sizeof(event_element_t)

static  event_element_t * _nodes = NULL;

event_element_t * event_element_new(int fd, uint32_t events)
{
    INFO("event_element_new\n");
    event_element_t *elem = calloc(1, event_element_s);
    if (elem)
    {
        elem->fd = fd;
        elem->events = events;
    }
    return elem;
}

void event_element_delete(event_element_t * elem)
{
    INFO("event_element_delete\n");
    free(elem);
}



//----------------------------event_manager-----------------------------

static event_manager_t event_manager;

event_manager_t * event_manager_init(int timeout)
{
    INFO("event_manager_init\n");
    
    event_manager.timeout = timeout;
    event_manager.epoll_fd = epoll_create(MAX_EVENTS);
    
    return &event_manager;
}


void event_manager_delete(void)
{
    close(event_manager.epoll_fd);
}


int event_manager_add_element(int fd, uint32_t flags, uint8_t cb_flags)
{
    event_element_t *elem = NULL;
    HASH_FIND_INT(_nodes,&fd,elem);
    if (elem)
    {
        LOG("fd (%d) already added updating flags\n", fd);
        elem->events |= flags;
        elem->cb_flags = cb_flags;
        
        struct epoll_event ev;
        memset(&ev, 0, sizeof(struct epoll_event));
        ev.data.fd = fd;
        ev.events = elem->events;
        
        return epoll_ctl(event_manager.epoll_fd, EPOLL_CTL_MOD, fd, &ev);
    }
    else
    {
        elem = event_element_new(fd, flags);
        elem->cb_flags = cb_flags;
       
        HASH_ADD_INT(_nodes,fd,elem);
        LOG("Added fd(%d)\n", fd);
        
        struct epoll_event ev;
        memset(&ev, 0, sizeof(struct epoll_event));
        ev.data.fd = fd;
        ev.events = elem->events;       
        
        return epoll_ctl(event_manager.epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    }
}


int event_manager_remove_element(int fd)
{
    
    INFO("event_manager_remove_element\n");
    
    event_element_t *elem = NULL;
    
    HASH_FIND_INT(_nodes,&fd,elem);
    
    if(elem)
    {
        //hulonghui
        HASH_DEL(_nodes,elem);
   	    close(fd);
    	epoll_ctl(event_manager.epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        
        event_element_delete(elem);
    }
    return 0;
}


int event_manager_process(void)
{
    struct epoll_event events[MAX_EVENTS];
    
    //LOG("May the source be with you!!%d\n",event_manager.count);
    int fds = epoll_wait(event_manager.epoll_fd, events, MAX_EVENTS, event_manager.timeout);
    
    if (fds == 0)
    {
        //INFO("event loop timed out\n");
        event_manager.count++;
    }
    
    int i = 0;
    for(;i<fds;i++)
    {
        event_element_t * value = NULL;
        HASH_FIND_INT(_nodes,&events[i].data.fd,value);
        
        if (value)
        {
            LOG("started processing for event id(%d) and sock(%d)\n", i, events[i].data.fd);
            // when data avaliable for read or urgent flag is set
            if ((events[i].events & EPOLLIN) || (events[i].events & EPOLLPRI))
            {
                /// connect or accept callbacks also go through EPOLLIN
                /// accept callback if flag set
                if (value->cb_flags & ACCEPT_CB)
                    accept_cb(value->fd);
                
/*
                /// connect callback if flag set
                if (value->cb_flags & CONNECT_CB)
                    connect_cb(value->fd);
*/
                /// read callback in any case
                read_cb(value->fd,&(value->data));
            }
/*
            // when write possible
            if (events[i].events & EPOLLOUT)
            {
                LOG("found EPOLLOUT for event id(%d) and sock(%d)\n", i, events[i].data.fd);
                
                if (write_cb){
                    write_cb(value->fd);
                }
            }
*/
            // shutdown or error
            if ( (events[i].events & EPOLLRDHUP) || (events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP))
            {                
                close_cb(value->fd,value->data);
            }
        }
        else // not in table
        {
            LOG("WARNING: NOT FOUND hash table value for event id(%d) and sock(%d)\n", i, events[i].data.fd);
        }
    } // for
    return 0;
}




