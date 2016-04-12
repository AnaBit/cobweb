#ifndef _ANABIT_COBWEB_BASE_TUNNEL_HPP_
#define _ANABIT_COBWEB_BASE_TUNNEL_HPP_

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

class io_epoll;

class tunnel {
public:
    enum class STATUS{
        AGAIN,
        FINISH,
        CLOSE
    };

public:
    tunnel(socket fd);
    virtual ~tunnel();

public:
    // user
    void usr_send(atom_buffer::buffer_data && buf);
    atom_buffer::buffer_data usr_revc();

public:
    // server
    virtual STATUS send();
    virtual STATUS recv();

    socket & sock();

private:
    socket _socket;

private:
    atom_buffer::buffer_data _buf;
    int _send_len;
    atom_buffer _buffer_send;
    atom_buffer _buffer_recv;
};

}
}

#endif // _ANABIT_COBWEB_BASE_TUNNEL_HPP_
