#ifndef _ANABIT_COBWEB_BASE_LOG_HPP_
#define _ANABIT_COBWEB_BASE_LOG_HPP_

#include <stdio.h>

namespace cobweb {
namespace base {

class log {
public:

};

}

}


// font attr
#define BOLD      "\033[1m"
// font color
#define BLACK     "\033[1;30m"
#define RED       "\033[1;31m"
#define GREEN     "\033[1;32m"
#define YELLOW    "\033[1;33m"
#define BLUE      "\033[1;34m"
#define FUCHSIA   "\033[1;35m"
#define CYAN      "\033[1;36m"
#define WHITE     "\033[1;37m"
// font attr end
#define ATTR_END  "\033[m"

#define MARK(symbol, fmt,  ...)                                                      \
    do {                                                                             \
        printf(BOLD "[ " symbol ATTR_END BOLD " ] - "                                \
            "{ " FUCHSIA fmt ATTR_END BOLD " } " ATTR_END " \n", ##__VA_ARGS__);     \
    } while(0)

#define ERROR_MARK(symbol, fmt,  ...)                                                \
    do {                                                                             \
        if (symbol == 1) {                                                           \
            printf(BOLD "[ " RED #symbol ATTR_END BOLD " ] - "                       \
                "{ " FUCHSIA fmt ATTR_END BOLD " } " ATTR_END " \n", ##__VA_ARGS__); \
        }                                                                            \
    } while(0)

#define WARNING_MARK(symbol, fmt,  ...)                                              \
    do {                                                                             \
        if (symbol == 1) {                                                           \
            printf(BOLD "[ " CYAN #symbol ATTR_END BOLD " ] - "                      \
                "{ " FUCHSIA fmt ATTR_END BOLD " } " ATTR_END " \n", ##__VA_ARGS__); \
        }                                                                            \
    } while(0)

#define RUN_MARK(symbol, fmt,  ...)                                                  \
    do {                                                                             \
        if (symbol == 1) {                                                           \
            printf(BOLD "[ " GREEN #symbol ATTR_END BOLD " ] - "                     \
                "{ " FUCHSIA fmt ATTR_END BOLD " } " ATTR_END " \n", ##__VA_ARGS__); \
        }                                                                            \
    } while(0)

#define FUNC_MARK(symbol)                                                            \
    do {                                                                             \
        if (symbol == 1) {                                                           \
            printf(BOLD "[ " BLUE #symbol ATTR_END BOLD " ] - "                      \
                "{ " FUCHSIA "%s" ATTR_END BOLD " } " ATTR_END " \n", __func__);     \
        }                                                                            \
    } while(0)

#endif // _ANABIT_COBWEB_BASE_LOG_HPP_
