#ifndef _ANABIT_COBWEB_BASE_SYS_CALL_HPP_
#define _ANABIT_COBWEB_BASE_SYS_CALL_HPP_

#include <sys/syscall.h>
#include <unistd.h>

namespace cobweb {
namespace base {

inline pid_t gettid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

inline pid_t getpid() {
    return ::getpid();
}

inline pid_t getppid() {
    return ::getppid();
}

inline bool in_thread() {
    return gettid() != cobweb::base::getpid();
}

}
}

#endif // _ANABIT_COBWEB_BASE_SYS_CALL_HPP_
