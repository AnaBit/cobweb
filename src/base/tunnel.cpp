#include "tunnel.hpp"

#include <sys/socket.h>
#include <iostream>

#include <algorithm>
#include <assert.h>

namespace cobweb {
namespace base {

tunnel::tunnel(socket fd)
    : _socket(fd),
      _send_len(0)
{

}

tunnel::~tunnel()
{
    std::cout << _socket.fd() << " close" << std::endl;
    _socket.close();
}

void tunnel::usr_send(atom_buffer::buffer_data && buf)
{
    _buffer_send.push_back(std::move(buf));
}

atom_buffer::buffer_data tunnel::usr_revc()
{
    return _buffer_recv.pop_front();
}

tunnel::STATUS tunnel::send()
{
    std::function<STATUS(void *, size_t)> func_send
            = [this] (void * buf, size_t len) -> STATUS{
        int ret = _socket.send(buf, len);
        if (ret < 0) {
            if (_socket.error_send(ret)) {
                return STATUS::CLOSE;
            } else {
                return STATUS::AGAIN;
            }
        }

        if (ret != (static_cast<int>(_buf.size()) - _send_len)) {
            _send_len += ret;
            return STATUS::AGAIN;
        }

        return STATUS::FINISH;
    };

    if (_buf.size() != 0) {
        func_send(&(_buf.at(_send_len)), _buf.size());
    }

    for (;;) {
        _buf = std::move(_buffer_send.pop_front());
        if (_buf.size() == 0) {
            _send_len = 0;
            return STATUS::FINISH;
        }

        STATUS state =  func_send(&(_buf.at(0)), _buf.size());
        if (state != STATUS::FINISH) {
            return state;
        }
    }
}

tunnel::STATUS tunnel::recv()
{
    for (;;) {
        atom_buffer::buffer_data buf;
        buf.resize(256);
        int ret = _socket.recv(&(buf.at(0)), buf.size());
        if (_socket.error_recv(ret)) {
            return tunnel::STATUS::CLOSE;
        }

        if (ret > 0) {
            buf.resize(ret);
            _buffer_recv.push_back(std::move(buf));
        }
    }
    return tunnel::STATUS::FINISH;
}

socket & tunnel::sock()
{
    return _socket;
}

}
}
