#ifndef _ANABIT_COBWEB_BASE_BUFFER_HPP_
#define _ANABIT_COBWEB_BASE_BUFFER_HPP_

#include <list>
#include <vector>
#include <memory>
#include <atomic>

namespace cobweb {
namespace base {

class atom_buffer {
public:
    using buffer_data = std::vector<unsigned char>;
public:
    atom_buffer();
    virtual ~atom_buffer();

    void push_back(buffer_data & data);
    void push_back(buffer_data && data);

    buffer_data front();
    buffer_data pop_front();
    void clear();
    size_t size();

    static buffer_data conv_data(std::vector<unsigned char> & data);
    static buffer_data conv_data(std::vector<unsigned char> && data);

private:
    void wait();
    void leave();

private:
    std::list<buffer_data> _buffer;
    std::atomic_flag _flag_buffer = ATOMIC_FLAG_INIT;
};

}
}

#endif // _ANABIT_COBWEB_BASE_BUFFER_HPP_
