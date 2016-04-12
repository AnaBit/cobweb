#ifndef _ANABIT_COBWEB_TCP_CONNECT_HPP_
#define _ANABIT_COBWEB_TCP_CONNECT_HPP_

#include "base/event_poll.hpp"

namespace cobweb {
namespace tcp {

class server
{
public:
    using io_epoll = base::io_epoll;
    using tunnel = base::tunnel;
    using socket = base::socket;
    using tunnel_ptr = std::unique_ptr<tunnel>;
    using buffer = std::vector<unsigned char>;
    using recv_callback = std::function<void(int, buffer &&)>;

public:
    server(std::string port) throw(std::runtime_error);
    virtual ~server();

    void start();
    void stop();

    void set_buf_size(size_t size);

    void set_recv_event(recv_callback && ev);
    void send(int fd, buffer & buf);

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
};

class client
{

};

}
}

#endif // _ANABIT_COBWEB_TCP_CONNECT_HPP_
