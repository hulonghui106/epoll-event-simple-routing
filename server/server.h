//
//  server.h
//  poll
//
//  Created by Fantem79 on 14/11/7.
//
//

#ifndef __poll__server__
#define __poll__server__

void read_cb(int socket_fd,void **data);
void close_cb(int socket_fd,void *data);
void accept_cb(int socket_fd);

/*
void connect_cb(int socket_fd);
void write_cb(int socket_fd);
*/
#endif /* defined(__poll__server__) */
