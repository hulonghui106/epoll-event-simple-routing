
#include<stdio.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<signal.h>

#define BUFFSIZE 1024

int main()
{
    //如果尝试send到一个disconnected socket上，就会让底层抛出一个SIGPIPE信号。这里防止进程退出
    //http://blog.sina.com.cn/s/blog_502d765f0100kopn.html
    //SIGPIPE handle by kernel	
    struct sigaction sa;
    sa.sa_handler=SIG_IGN;
    sigaction(SIGPIPE,&sa,0);

    // create a TCP socket, bind and listen
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in svr_addr;
    memset(&svr_addr, 0 , sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = htons(INADDR_ANY); //自动获得本机IP
    svr_addr.sin_port = htons(8080);  //端口设置为8080

    bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr));
    listen(sock, 10);

    //设置为非阻塞 http://www.cnblogs.com/andtt/articles/2178875.html
    fcntl(sock, F_SETFL, O_NONBLOCK);

    // create a poll event object, with time out of 1 sec
    poll_event_t *pe = poll_event_new(1000);
    // set timeout callback
    pe->timeout_callback = timeout_cb;
    poll_event_element_t *p;
    // add sock to poll event
    poll_event_add(pe, sock, EPOLLIN, &p);
    // set callbacks
    //p->read_callback = read_cb;
    p->accept_callback = accept_cb;
    p->close_callback = close_cb;
    // enable accept callback
    p->cb_flags |= ACCEPT_CB;
    // start the event loop
    use_the_force(pe);

    return 0;
}