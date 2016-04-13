#ifndef _ANABIT_COBWEB_BASE_EVENT_POLL_HPP_
#define _ANABIT_COBWEB_BASE_EVENT_POLL_HPP_

#include <map>
#include <queue>
#include <vector>
#include <mutex>
#include <memory>
#include <atomic>
#include <functional>

#include "thread_pool.hpp"
#include "io_epoll.hpp"
#include "buffer.hpp"
#include "net.hpp"

namespace cobweb {
namespace base {

// ----------------------------------------------------------------------------

class event_poll {
public:
    using io_data = io_epoll::io_data;
    using event_func = std::function<void(void)>;
    using event_callback = std::function<void(io_data *)>;

public:
    event_poll();
    virtual ~event_poll();

    // usr api
    void start();
    void wait();
    void stop();

    // inter use
    void push_event(event_func && ev);

    bool add(int fd, io_data * tun);
    bool remove(int fd);

    bool in_event(int fd, io_data * tun);
    bool out_event(int fd, io_data * tun);

    // call back
    void set_remove_callback(event_callback && ev);
    void set_send_callback(event_callback && ev);
    void set_recv_callback(event_callback && ev);

private:
    void handle_event();

private:
    // thread runing state
    bool looping();

    // poll event
    void handle_poll();

    // event handle
    void event_recv(io_data * io);
    void event_send(io_data * io);
    void event_remove(io_data * io);

private:
private:
    event_callback _event_remove_callback;
    event_callback _event_revc_callback;
    event_callback _event_send_callback;

private:
    // thread
    std::thread _loop;
    std::atomic_flag _run_loop_flag = ATOMIC_FLAG_INIT;

private:
    // thread handle/.
    thread_pool _pool;
    std::mutex _mutex_pending_event;
    std::list<std::function<void(void)>> _pending_events;

    // epoll attr
    io_epoll _poll;
    io_epoll::event_list _event;

    static const size_t INIT_SIZE = 16;
};

}
}

#endif // _ANABIT_COBWEB_BASE_EVENT_POLL_HPP_
