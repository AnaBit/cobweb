#include "buffer.hpp"
namespace cobweb {
namespace base {

buffer::buffer_data buffer::conv_data(std::vector<unsigned char> & data)
{
    buffer_data buf = std::make_shared<std::vector<unsigned char>>(data);
    return buf;
}

buffer::buffer_data buffer::conv_data(std::vector<unsigned char> && data)
{
    buffer_data buf = std::make_shared<std::vector<unsigned char>>(data);
    return buf;
}

void buffer::push_back(buffer::buffer_data & data)
{
    _buffer.push_back(data);
}

void buffer::push_back(buffer::buffer_data && data)
{
    _buffer.push_back(data);
}

buffer::buffer_data buffer::front()
{
    return _buffer.front();
}

buffer::buffer_data buffer::pop_front()
{
    buffer_data buf;
    _buffer.pop_front();
    return buf;
}

void buffer::clear()
{
    _buffer.clear();
}

size_t buffer::size()
{
    _buffer.size();
}

}
}
