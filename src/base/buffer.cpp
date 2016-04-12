#include "buffer.hpp"
namespace cobweb {
namespace base {

atom_buffer::buffer_data atom_buffer::conv_data(std::vector<unsigned char> & data)
{
    buffer_data buf(data);
    return buf;
}

atom_buffer::buffer_data atom_buffer::conv_data(std::vector<unsigned char> && data)
{
    buffer_data buf(data);
    return buf;
}

atom_buffer::atom_buffer()
{

}

atom_buffer::~atom_buffer()
{

}

void atom_buffer::push_back(atom_buffer::buffer_data & data)
{
    wait();
    _buffer.push_back(data);
    leave();
}

void atom_buffer::push_back(atom_buffer::buffer_data && data)
{
    wait();
    _buffer.push_back(data);
    leave();
}

atom_buffer::buffer_data atom_buffer::front()
{
    wait();
    buffer_data buf = _buffer.front();
    leave();
    return buf;
}

atom_buffer::buffer_data atom_buffer::pop_front()
{
    wait();
    buffer_data buf = _buffer.front();
    _buffer.pop_front();
    leave();
    return buf;
}

void atom_buffer::clear()
{
    wait();
    _buffer.clear();
    leave();
}

size_t atom_buffer::size()
{
    wait();
    size_t size = _buffer.size();
    leave();
    return size;
}

void atom_buffer::wait()
{
    while (_flag_buffer.test_and_set());
}

void atom_buffer::leave()
{
    _flag_buffer.clear();
}

}
}
