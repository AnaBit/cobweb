#include "io_epoll.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>

#include <cassert>

#include "base/log.hpp"
#include "config.hpp"

#define IO_EPOLL __DEBUG_COBWEB

namespace anabit {
io_epoll::io_epoll() throw(std::runtime_error)
    : _fd(epoll_create1(EPOLL_CLOEXEC)),
      _event(INIT_EVENTS)
{
    if (_fd == -1) {
        throw std::runtime_error("epoll create error");
    }
}

io_epoll::~io_epoll()
{
    ::close(_fd);
}

void io_epoll::poll(int timeout_ms)
{
    int event_num = epoll_wait(_fd, &_event.at(0),
                            static_cast<int>(_event.size()),
                            timeout_ms);
    if (event_num > 0) {
        if (static_cast<size_t>(event_num) == _event.size()) {
            _event.resize(_event.size() * 2);
        }
    } else if (event_num == 0){
        // if timeout_ms == -1 the epoll_wait return with event_num > 0;

    } else {
        ERROR_MARK(IO_EPOLL, "run error");
    }
}

void io_epoll::add(net_pipe & pipe)
{
    (void) pipe;
}

void io_epoll::modify(net_pipe & pipe)
{
    (void) pipe;
}

void io_epoll::remove(net_pipe & pipe)
{
    (void) pipe;
}

}

