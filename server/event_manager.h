#ifndef _EVENT_MANAGER_H_HLH
#define _EVENT_MANAGER_H_HLH


#define MAX_EVENTS 100

#define ACCEPT_CB 0x01
#define CONNECT_CB 0x02

struct event_manager_st
{
    /** timeout duration */
    size_t timeout;
    /** epoll file descriptor*/
    int epoll_fd;
    /** user data for poll_event */
    int count;
};

typedef struct event_manager_st event_manager_t;


event_manager_t * event_manager_init(int);
void poll_event_delete(void);

int event_manager_add_element(int fd, uint32_t flags, uint8_t cb_flags);
int event_manager_remove_element(int fd);

int event_manager_process(void);

#endif
