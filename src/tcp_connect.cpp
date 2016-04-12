#include "tcp_connect.hpp"

#include <iostream>

#include <string.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include "log.hpp"
#include "config.hpp"

#define TCP_SERVER __COBWEB_DEBUG_ON

namespace cobweb {
namespace tcp {

server::server(std::string port) throw(std::runtime_error)
    try : _port(port)
{
    set_callback();
    socket sock;
    sock.set_no_block(socket::YES_NO::YES);

    socket::alive_con attr;
    attr.alive = 1;
    attr.idle_time_s = 5;
    attr.resend_time_s = 1;
    attr.resend_cnt = 3;
    sock.set_keep_alive(attr);
    sock.set_reuse_addr(socket::YES_NO::YES);
    sock.set_socket_no_delay(socket::YES_NO::YES);

    sockaddr_in addr;
    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8000);

    if (sock.bind(addr) < 0
            ||  sock.listen() < 0) {
        throw std::runtime_error("bind or listen");
    }

    listen(sock);

    _socket.set_fd(sock.fd());

} catch (std::runtime_error & e) {
    ERROR_MARK(TCP_SERVER, "%s", e.what());
}

server::~server()
{

}

void server::start()
{
    _poll.start();
}

void server::stop()
{
    _poll.stop();
}

void server::set_buf_size(size_t size)
{
    if (size < 32) {
        size = 32;
    }
    _buf_size = size;
}

void server::set_recv_event(recv_callback && ev)
{
    _recv_callback = ev;
}

void server::send(int fd, server::buffer & buf)
{
    socket sock(fd);
    sock.send(&(buf.at(0)), buf.size());
    _poll.push_event([] () {

    });
}

void server::set_callback()
{
    _poll.set_recv_callback([this] (tunnel * tun) {
        if (tun->sock().fd() == _socket.fd()) {
            sockaddr_in addr;
            socket sock(_socket.accept(addr));
            if (sock.fd() > 0) {
                tunnel * clinet = new tunnel(sock);
                _poll.add(sock.fd(), clinet);
                std::cout << "accept" << std::endl;
            }
        } else {
            buffer buf;
            socket &sock = tun->sock();
            buf.resize(_buf_size);
            int ret = sock.recv(&(buf.at(0)), buf.size());
            if (sock.error_recv(ret) || ret == 0) {
                _poll.push_event([this, sock, tun] () mutable {
                    _poll.remove(sock.fd());
                    delete tun;
                    std::cout << "close" << std::endl;
                });
            } else {
                buf.resize(ret);
                send(sock.fd(), buf);
                if (_recv_callback) {
                    _recv_callback(sock.fd(), std::move(buf));
                }
            }
        }
    });

    _poll.set_send_callback([this] (tunnel * tun) {

    });
}

bool server::listen(server::socket & s)
{
    tunnel_ptr ptr(new tunnel(s));
    _master_tun = std::move(ptr);
    _poll.add(_master_tun->sock().fd(), &(*_master_tun));
    return true;
}



}
}
