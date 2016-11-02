#include "event.h"
#include <sys/event.h>
#include <cstring>
#include <iostream>

event::event(int fd)
: fd_(fd)
{
    events_ = new struct kevent[2];
    data_ = kqueue(); 
    monitor_ = new struct kevent;
    struct kevent &monitor = *static_cast<struct kevent*>(monitor_);
    EV_SET(&monitor, fd_, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
}

event::~event()
{
    if (events_)
    {
        delete[] static_cast<struct kevent*>(events_);
        events_ = 0;
    }

    if (monitor_)
    {
        delete static_cast<struct kevent*>(monitor_);
        monitor_ = 0;
    }
}

bool event::wait(uint32_t timeout_usec)
{
    struct kevent *monitor = static_cast<struct kevent*>(monitor_);
    struct kevent *events = static_cast<struct kevent*>(events_);
    int event_count = kevent(data_, monitor, 1, events, 1, 0);
    if (event_count < 0)
    {
        std::cerr << "Error waiting for events: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}
