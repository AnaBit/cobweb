#ifndef _ANABIT_COBWEB_BASE_BUFFER_HPP_
#define _ANABIT_COBWEB_BASE_BUFFER_HPP_

#include <list>
#include <vector>
#include <memory>

namespace cobweb {
namespace base {

class buffer {
public:
    using buffer_data = std::shared_ptr<std::vector<unsigned char>>;
public:
    buffer() {}
    virtual ~buffer();

    void push_back(buffer_data & data);
    void push_back(buffer_data && data);

    buffer_data front();
    buffer_data pop_front();
    void clear();
    size_t size();

    static buffer_data conv_data(std::vector<unsigned char> & data);
    static buffer_data conv_data(std::vector<unsigned char> && data);

private:
    std::list<buffer_data> _buffer;
};

}
}

#endif // _ANABIT_COBWEB_BASE_BUFFER_HPP_
