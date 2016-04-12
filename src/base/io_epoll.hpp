#ifndef _ANABIT_COBWEB_BASE_IO_EPOLL_HPP_
#define _ANABIT_COBWEB_BASE_IO_EPOLL_HPP_

#include <sys/epoll.h>

#include <exception>
#include <stdexcept>
#include <vector>

namespace cobweb {
namespace base {

class io_epoll {
public:
    using io_data = void;
    using event = epoll_event;
    using event_list = std::vector<event>;

    enum class FD_EVENT : uint32_t {
        FD_IN = EPOLLIN,
        FD_OUT = EPOLLOUT
    };

    enum class TRIGGER : uint32_t {
        // level
        LT = 0,
        // egle
        ET = EPOLLET
    };

public:
    static bool ev_error(event &ev);
    static bool ev_recv(event &ev);
    static bool ev_send(event &ev);

    template <class T>
    static uint32_t conv_enum(T t);

public:
    // level tigger
    io_epoll(TRIGGER trigger = TRIGGER::LT) throw(std::runtime_error);
    virtual ~io_epoll();

    int poll(event_list &ev, int timeout_ms = -1) const;

    bool add(int sfd, io_data * pipe) const;
    bool modify(int sfd, io_data * pipe, FD_EVENT state) const;
    bool remove(int sfd) const;

private:
    int _ep_fd;
    TRIGGER _t;
};

template <class T>
uint32_t io_epoll::conv_enum(T t) {
    return static_cast<uint32_t>(t);
}

}
}

#endif // _ANABIT_COBWEB_BASE_IO_EPOLL_HPP_
