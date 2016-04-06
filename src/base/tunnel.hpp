#ifndef _ANABIT_COBWEB_BASE_TUNNEL_HPP_
#define _ANABIT_COBWEB_BASE_TUNNEL_HPP_

#include <functional>
#include <queue>
#include <vector>
#include <mutex>
#include <memory>
#include <atomic>

#include "thread_pool.hpp"

namespace cobweb {
namespace base {

class tunnel {
public:
    class tunnel_event {
        tunnel_event();
        ~tunnel_event();
    };

    enum class link_state {
        disconnect,
        connected
    };

    using buffer = std::vector<unsigned char>;
    using event_handle_func = std::function<void(tunnel_event &event)>;

public:
    tunnel(int fd, std::shared_ptr<thread_pool> &pool);
    virtual ~tunnel();

public:
    void send(buffer & buf);
    void send(buffer && buf);

    // tunnel event handle
    void set_noti_handle_func(event_handle_func & func);
    void set_noti_handle_func(event_handle_func && func);

private:
    void send_loop();
    void send_handle(void *buffer, size_t len);

    void push_send_buffer(buffer & buf);
    void push_send_buffer(buffer && buf);

private:

    int _fd;
    event_handle_func _event_func;
    std::shared_ptr<thread_pool> _pool;

    std::atomic_flag _atomic_in_thread = ATOMIC_FLAG_INIT;
    std::atomic<link_state> _atomic_link_state {link_state::disconnect};

private:
    std::mutex _mut_que_send;
    std::queue<buffer> _que_send;

    std::mutex _mut_que_read;
    std::queue<buffer> _que_read;
};

class tunnel_handle {
public:
    using tunnel_set = std::vector<std::weak_ptr<tunnel>>;

public:
    tunnel_handle();
    virtual ~tunnel_handle();

private:
    void handle_loop();

private:
    std::thread _loop;

    thread_pool _pool;
};

}
}

#endif // _ANABIT_COBWEB_BASE_TUNNEL_HPP_
