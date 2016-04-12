#include <iostream>
#include <vector>
#include <chrono>

#include "unistd.h"
using namespace std;

#include "src/base/thread_pool.hpp"
#include "src/tcp_connect.hpp"

using namespace cobweb::tcp;

void recv_callback(int fd, server::buffer && buf)
{
    printf("%d   %ld\r\n", fd, buf.size());
}

int main()
{

//    cobweb::base::thread_pool pool(std::thread::hardware_concurrency() * 2);
//    std::vector< std::future<int> > results;

//    for(int i = 0; i < 8; ++i) {
//        results.emplace_back(
//            pool.enqueue([i] {
//                std::cout << "hello " << i << std::endl;
//                std::this_thread::sleep_for(std::chrono::seconds(1));
//                std::cout << "world " << i << std::endl;
//                return i*i;
//            })
//        );
//    }

//    for(auto && result: results)
//        std::cout << result.get() << ' ';
//    std::cout << std::endl;

    cobweb::tcp::server ser("8000");
    ser.set_recv_event([] (int fd, server::buffer && buf) {
        recv_callback(fd, std::move(buf));
    });
    ser.start();

    pause();
    return 0;
}

