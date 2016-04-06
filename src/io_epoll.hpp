#ifndef _ANABIT_IO_EPOLL_HPP_
#define _ANABIT_IO_EPOLL_HPP_

#include <sys/epoll.h>

#include <exception>
#include <stdexcept>
#include <vector>

#include "net_pipe.hpp"

struct epoll_event;

namespace anabit {

class io_epoll {
public:
    io_epoll() throw(std::runtime_error);
    virtual ~io_epoll();

    void poll(int timeout_ms);

    void add(net_pipe & pipe);
    void modify(net_pipe & pipe);
    void remove(net_pipe & pipe);

private :
    void event_handle(int event_num);

private :
    using event = std::vector<epoll_event>;

private:
    int _fd;

    const int INIT_EVENTS = 16;

    event _event;
};

}

#endif // _ANABIT_IO_EPOLL_HPP_
