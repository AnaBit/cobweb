#include "event_poll.hpp"

#include <sys/socket.h>

#include <algorithm>
#include <iostream>
#include <assert.h>
#include <unistd.h>

namespace cobweb {
namespace base {

event_poll::event_poll()
    : _pool(std::thread::hardware_concurrency() * 2)
{
    _event.resize(INIT_SIZE);
}

event_poll::~event_poll()
{

}

void event_poll::start()
{
    looping();
    std::thread loop([this](){
        handle_poll();
    });
    loop.detach();
}

void event_poll::wait()
{
    while (looping() == true);
}

void event_poll::stop()
{
    _run_loop_flag.clear();
}

void event_poll::push_event(event_func && ev)
{
    {
        std::unique_lock<std::mutex> lock(_mutex_pending_event);
        _pending_events.push_back(ev);
    }
}

bool event_poll::add(int fd, io_data * tun)
{
    return _poll.add(fd, tun);
}

bool event_poll::remove(int fd)
{
    return _poll.remove(fd);
}

bool event_poll::in_event(int fd, io_data * tun)
{
    return _poll.modify(fd, tun, io_epoll::FD_EVENT::FD_IN);
}

bool event_poll::out_event(int fd, io_data * tun)
{
    return _poll.modify(fd, tun, io_epoll::FD_EVENT::FD_OUT);
}

void event_poll::set_remove_callback(event_callback && ev)
{
    _event_remove_callback = ev;
}

void event_poll::set_send_callback(event_callback && ev)
{
    _event_send_callback = ev;
}

void event_poll::set_recv_callback(event_callback && ev)
{
    _event_revc_callback = ev;
}

void event_poll::handle_event()
{
    std::list<std::function<void(void)>> active_event;
    {
        std::unique_lock<std::mutex> lock(_mutex_pending_event);
        active_event.swap(_pending_events);
    }

    for (auto func : active_event) {
        func();
    }
}

bool event_poll::looping()
{
    return _run_loop_flag.test_and_set();
}

void event_poll::handle_poll()
{
    int event_cnt;
    while (looping()) {
        event_cnt = _poll.poll(_event, 1000);
        while (event_cnt > 0) {
            io_epoll::event ev =  _event.at(static_cast<size_t>(--event_cnt));
            io_data * tun = (io_data *)ev.data.ptr;
            if (io_epoll::ev_error(ev)) {
                event_remove(tun);
                continue;
            } else {

                if (io_epoll::ev_recv(ev)) {
                    event_recv(tun);
                }

                if (io_epoll::ev_send(ev)) {
                    event_send(tun);
                }
            }
        }

        handle_event();
    }
}

void event_poll::event_recv(io_data * io)
{
    if (_event_revc_callback) {
        _event_revc_callback(io);
    }
}

void event_poll::event_send(io_data * io)
{
    _event_send_callback(io);
}

void event_poll::event_remove(io_data * io)
{
    _event_remove_callback(io);
}

}
}
