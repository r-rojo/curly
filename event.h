#pragma once
#include <cstdint>

class event
{
    public:
        event(){}
        event(int fd);
        ~event();

        bool wait(uint32_t timeout_usec);
    private:
        int fd_;
        void *events_;
        void *monitor_;
        int data_;
};
