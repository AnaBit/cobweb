#include "net.hpp"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include "config.hpp"
#include "log.hpp"
#include "net.hpp"


#define SOCKET_TCP __COBWEB_DEBUG_ON

namespace cobweb {
namespace base {

socket::socket() throw(std::runtime_error)
{
    _fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_fd == -1) {
        ERROR_MARK(SOCKET_TCP, "socket erron = %d", errno);
        throw(std::runtime_error("socket create error!"));
    }
}

socket::socket(socket & sock)
{
    _fd = sock._fd;
}

socket::socket(socket && sock)
{
    _fd = sock._fd;
    sock._fd = 0;
}

socket::socket(int fd)
{
    _fd = fd;
}

socket::~socket()
{

}

int socket::bind(const sockaddr_in & addr)
{
   int ret = ::bind(_fd, (sockaddr *)&addr, sizeof(sockaddr));
   if (ret < 0) {
       ERROR_MARK(SOCKET_TCP, "bind erron = %d", errno);
   }

   return ret;
}

int socket::listen(int n)
{
    int ret = ::listen(_fd, n);
    if (ret < 0) {
        ERROR_MARK(SOCKET_TCP, "listen erron = %d", errno);
    }

    return ret;
}

int socket::accept(sockaddr_in & addr)
{
    socklen_t len = sizeof(sockaddr);
    int p_fd = ::accept(_fd, (sockaddr *)&addr, &len);
    if (p_fd < 0) {
        ERROR_MARK(SOCKET_TCP, "%s erron = %d", __func__, errno);
    }
    return p_fd;
}

int socket::send(void * buf, size_t len)
{
    return static_cast<int>(::send(_fd, buf, len, 0));
}

bool socket::error_send(int send_ret)
{
    if (send_ret >= 0) {
        return false;
    }

    if (errno == EINTR || errno == EAGAIN) {
        return false;
    }

    return true;
}

int socket::recv(void * buf, size_t len)
{
    return static_cast<int>(::recv(_fd, buf, len, 0));
}

bool socket::error_recv(int ret_recv)
{
    if (ret_recv >= 0) {
        return false;
    }

    if (errno == EAGAIN) {
        return false;
    }

    return true;
}

int socket::fd()
{
    return _fd;
}

void socket::set_fd(int fd)
{
    _fd = fd;
}

void socket::close()
{
    ::close(_fd);
}

void socket::set_reuse_addr(YES_NO state)
{
    int ops = 0;
    if (state == YES_NO::YES) {
        ops = 1;
    } else {
        ops = 0;
    }

    if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &ops, sizeof(ops)) < 0) {
        ERROR_MARK(SOCKET_TCP, "%s erron = %d", __func__, errno);
    }
}

void socket::set_reuse_port(YES_NO state)
{
    int ops = 0;
    if (state == YES_NO::YES) {
        ops = 1;
    } else {
        ops = 0;
    }

    if (::setsockopt(_fd,SOL_SOCKET, SO_REUSEPORT, &ops, sizeof(ops)) < 0) {
        ERROR_MARK(SOCKET_TCP, "%s erron = %d", __func__, errno);
    }
}

void socket::set_keep_alive(const alive_con &state)
{
    int ret = 0;
    ret += ::setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE,
                  &(state.alive), sizeof(state.alive));
    ret += ::setsockopt(_fd, SOL_TCP, TCP_KEEPIDLE,
                  &(state.idle_time_s), sizeof(state.idle_time_s));
    ret += ::setsockopt(_fd, SOL_TCP, TCP_KEEPINTVL,
                      &(state.idle_time_s), sizeof(state.idle_time_s));
    ret += ::setsockopt(_fd, SOL_TCP, TCP_KEEPCNT,
                      &(state.idle_time_s), sizeof(state.idle_time_s));
    if (ret != 0) {
        ERROR_MARK(SOCKET_TCP, "%s erron = %d", __func__, errno);
    }
}

void socket::set_socket_no_delay(YES_NO state)
{
    int ops;
    if (state == YES_NO::YES) {
        ops = 1;
    } else {
        ops = 0;
    }
    if (::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY,
                 &ops, static_cast<socklen_t>(sizeof ops)) < 0) {
        ERROR_MARK(SOCKET_TCP, "%s erron = %d", __func__, errno);
    }
}

void socket::set_no_block(YES_NO state)
{
    int flags = ::fcntl (_fd, F_GETFL, 0);

    if (flags < 0) {
        ERROR_MARK(SOCKET_TCP, "%s erron = %d", __func__, errno);
        return;
    }

    if (state == YES_NO::YES) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    if (::fcntl (_fd, F_SETFL, flags) < 0) {
        ERROR_MARK(SOCKET_TCP, "%s erron = %d", __func__, errno);
        return;
    }
}

void socket::set_close_exec()
{
    int flags = ::fcntl(_fd, F_GETFD, 0);

    if (flags < 0) {
        ERROR_MARK(SOCKET_TCP, "%s erron = %d", __func__, errno);
        return;
    }

    flags |= FD_CLOEXEC;

    if (::fcntl(_fd, F_SETFD, flags) < 0) {
        ERROR_MARK(SOCKET_TCP, "%s erron = %d", __func__, errno);
        return;
    }
}

}
}
