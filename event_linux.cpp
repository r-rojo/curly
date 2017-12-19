#include "event.h"
#include <stdlib.h>
#include <sys/epoll.h>
#include <stdio.h>

event::event(int fd)
: fd_(fd)
{
     struct epoll_event ev;
     events_ = new struct epoll_event[1];
     int listen_sock, conn_sock, nfds, epollfd;

     /* Set up listening socket, 'listen_sock' (socket(),
        bind(), listen()) */

     data_ = epoll_create(10);
     if (epollfd == -1) 
     {
         perror("epoll_create");
         exit(EXIT_FAILURE);
     }

     ev.events = EPOLLIN;
     ev.data.fd = fd;
     if (epoll_ctl(data_, EPOLL_CTL_ADD, fd, &ev) == -1) {
         perror("epoll_ctl: listen_sock");
         exit(EXIT_FAILURE);
     }

}

event::~event()
{
    delete [](struct epoll_event*)events_;
}

bool event::wait(uint32_t timeout_usec)
{
    auto nfds = epoll_wait(data_, static_cast<struct epoll_event*>(events_), 10, -1);
    if (nfds == -1) {
        perror("epoll_pwait");
        exit(EXIT_FAILURE);
    }
    return nfds > 0;
}
