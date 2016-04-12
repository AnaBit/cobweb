#ifndef _ANABIT_COBWEB_BASE_NET_HPP_
#define _ANABIT_COBWEB_BASE_NET_HPP_

// c header
#include <sys/socket.h>
// c++ header
#include <stdexcept>

struct sockaddr_in;

namespace cobweb {
namespace base {

class socket final{
public:
    enum class YES_NO {
        YES,
        NO
    };

    struct alive_con {
        int alive;
        int idle_time_s;
        int resend_time_s;
        int resend_cnt;
    };

public:
    socket() throw(std::runtime_error);
    socket(socket & sock);
    socket(socket && sock);
    socket(int fd);
    ~socket();

public:
    int bind(const sockaddr_in &addr);
    int listen(int n = SOMAXCONN);
    int accept(sockaddr_in &addr);
    int connect(const sockaddr_in &addr);

    int send(void * buf, size_t len);
    bool error_send(int send_ret);

    int recv(void * buf, size_t len);
    bool error_recv(int ret_recv);

    int fd();
    void set_fd(int fd);
    void close();

    void set_reuse_addr(YES_NO state);
    void set_reuse_port(YES_NO state);
    void set_keep_alive(const alive_con &state);
    void set_socket_no_delay(YES_NO state);
    void set_no_block(YES_NO state);
    void set_close_exec();

private:
    int _fd;

};

}
}

#endif // _ANABIT_COBWEB_BASE_NET_HPP_
