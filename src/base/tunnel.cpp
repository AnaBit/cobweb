#include "tunnel.hpp"

#include <sys/socket.h>

#include <algorithm>
#include <assert.h>

namespace cobweb {
namespace base {

tunnel::tunnel(int fd, std::shared_ptr<thread_pool> & pool)
    : _fd(fd),
      _pool(pool)
{

}

tunnel::~tunnel()
{

}

void tunnel::send(buffer & buf)
{
    if (_atomic_link_state == link_state::connected) {
        if (_atomic_in_thread.test_and_set()) {
            _pool->enqueue([this, buf]() mutable {
                push_send_buffer(std::move(buf));
            });
        } else {
            _pool->enqueue([this, buf]() mutable {
                push_send_buffer(std::move(buf));
                send_loop();
            });
        }
    }
}

void tunnel::send(buffer && buf)
{
    std::unique_lock<std::mutex> lock(_mut_que_send);
    _que_send.emplace(buf);
}

tunnel::tunnel_event::tunnel_event()
{

}

tunnel::tunnel_event::~tunnel_event()
{

}

void tunnel::set_noti_handle_func(event_handle_func & func)
{
    _event_func = func;
}

void tunnel::set_noti_handle_func(event_handle_func && func)
{
    _event_func = std::move(func);
}

void tunnel::send_loop()
{
    while (true) {
        buffer buf;
        {
            std::unique_lock<std::mutex> lock(_mut_que_send);
            if (!_que_send.empty()) {
                buf = std::move(_que_send.front());
                _que_send.pop();
            }
        }
        if (buf.size() == 0) break;
        send_handle((void *)&(*(buf.begin())), buf.size());
    }

    _atomic_in_thread.clear();
}

void tunnel::send_handle(void *buffer, size_t len)
{
    ::send(_fd, buffer, len, 0);
}

void tunnel::push_send_buffer(tunnel::buffer & buf)
{
    std::unique_lock<std::mutex> lock(_mut_que_send);
    _que_send.push(buf);
}

void tunnel::push_send_buffer(buffer && buf)
{
    std::unique_lock<std::mutex> lock(_mut_que_send);
    _que_send.push(buf);
}

tunnel_handle::tunnel_handle()
{

}

tunnel_handle::~tunnel_handle()
{

}

void tunnel_handle::handle_loop()
{
    while (true) {

    }
}

}
}
