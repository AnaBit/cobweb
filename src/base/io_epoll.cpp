#include "io_epoll.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>

#include <cassert>

#include "log.hpp"
#include "config.hpp"

#define IO_EPOLL __COBWEB_DEBUG_ON

namespace cobweb {
namespace base {

io_epoll::io_epoll(TRIGGER trigger) throw(std::runtime_error)
    : _ep_fd(epoll_create1(EPOLL_CLOEXEC)),
      _t(trigger)
{
    if (_ep_fd == -1) {
        throw std::runtime_error("epoll create error");
    }
}

io_epoll::~io_epoll()
{
    ::close(_ep_fd);
}

int io_epoll::poll(event_list &ev, int timeout_ms) const
{
    int event_num = epoll_wait(_ep_fd, &ev.at(0),
                            static_cast<int>(ev.size()),
                            timeout_ms);
    if (event_num > 0) {
        if (static_cast<size_t>(event_num) == ev.size()) {
            ev.resize(ev.size() * 2);
        }
    } else if (event_num == 0){
        // if timeout_ms == -1 the epoll_wait return with event_num > 0;

    } else {
        ERROR_MARK(IO_EPOLL, "%s", __func__);
    }

    return event_num;
}

bool io_epoll::add(int sfd, io_data * pipe) const
{
    epoll_event ev = {0, {0}};
    ev.events = conv_enum(FD_EVENT::FD_IN)
                | conv_enum(_t);
    ev.data.ptr = static_cast<void *>(pipe);
    if(::epoll_ctl(_ep_fd, EPOLL_CTL_ADD, sfd, &ev) == -1){
        ERROR_MARK(IO_EPOLL, "%s", __func__);
        return false;
    }
    return true;
}

bool io_epoll::modify(int sfd, io_data * pipe, FD_EVENT state) const
{
    epoll_event ev = {0, {0}};
    ev.events = conv_enum(state);
    ev.data.ptr = static_cast<void *>(pipe);
    if(::epoll_ctl(_ep_fd, EPOLL_CTL_MOD, sfd, &ev) == -1){
        ERROR_MARK(IO_EPOLL, "%s", __func__);
        return false;
    }
    return true;
}

bool io_epoll::remove(int sfd) const
{
    epoll_event ev = {0, {0}};
//    ev.data.ptr = static_cast<void *>(pipe);
    if(::epoll_ctl(_ep_fd, EPOLL_CTL_DEL, sfd, &ev) == -1){
        ERROR_MARK(IO_EPOLL, "%s", __func__);
        return false;
    }
    return true;
}

bool io_epoll::ev_error(io_epoll::event & ev)
{
    if (((ev.events & EPOLLERR) != 0)
            || ((ev.events & EPOLLHUP) != 0)) {
        return true;
    }

    return false;
}

bool io_epoll::ev_recv(io_epoll::event & ev)
{
    if ((ev.events & conv_enum(FD_EVENT::FD_IN)) != 0) {
        return true;
    }

    return false;
}

bool io_epoll::ev_send(io_epoll::event & ev)
{
    if ((ev.events & conv_enum(FD_EVENT::FD_OUT)) != 0) {
        return true;
    }

    return false;
}

}
}

