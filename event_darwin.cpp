#include "event.h"
#include <sys/event.h>
event::event(int fd)
: fd_(fd)
{
    events_ = new struct kevent[2];
    data_ = kqueue(); 
    monitor_ = new struct kevent;
    EV_SET(&monitor[0], fd_, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
}

event::~event()
{
    if (events_)
    {
        delete[] events_;
        events_ = 0;
    }

    if (monitor_)
    {
        delete monitor_;
        monitor_ = 0;
    }
}

bool event::wait(uint32_t timeout_usec)
{
    struct kevent *monitor = monitor_;
    struct kevent *events = events_;
    int event_count = kevent(data, monitor, 1 events, 1, NULL);
    if (event_count < 0)
    {
        std::cerr << "Error waiting for events: " << strerror(errno) << std::endl;
        return false;
    }

}
