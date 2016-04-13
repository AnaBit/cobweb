#ifndef _ANABIT_COBWEB_TCP_CONNECT_HPP_
#define _ANABIT_COBWEB_TCP_CONNECT_HPP_

#include "base/event_poll.hpp"
#include "base/tunnel.hpp"

namespace cobweb {
namespace tcp {

// ----------------------------------------------------------------------------
class server
{
public:
    using io_epoll = base::io_epoll;
    using io_data  = base::io_epoll::io_data;
    using tunnel = base::tunnel;
    using socket = base::socket;
    using buffer = std::vector<unsigned char>;
    using recv_callback = std::function<void(int, buffer &&)>;
    using tunnel_ptr = std::shared_ptr<tunnel>;

public:
    server(std::string port) throw(std::runtime_error);
    virtual ~server();

    void start();
    void stop();

    void set_buf_size(size_t size);

    void set_recv_event(recv_callback && ev);
    void send(int fd, server::buffer & buf);

private:
    void set_callback();

private:
    bool listen(socket & s);
    bool tunnel_add(socket & s);

private:
    recv_callback _recv_callback;

private:
    size_t _buf_size = 256;
    std::string _port;
    tunnel_ptr _master_tun;
    socket _socket;
    base::event_poll _poll;

    // modify tunnel
    std::mutex _mutex_tunnels;
    std::map<int, tunnel_ptr> _tunnels;
    using tun_itr = std::map<int, tunnel_ptr>::iterator;
};


class client {
public:
    using socket = base::socket;
    using tunnel = base::tunnel;
    using buffer = std::vector<unsigned char>;
    using io_data  = base::io_epoll::io_data;
    using callback = base::event_poll::event_callback;
public:
    client();
    ~client();

private:
    base::thread_pool _pool;
    base::event_poll _poll;
    socket _socket;
};

}
}

#endif // _ANABIT_COBWEB_TCP_CONNECT_HPP_
