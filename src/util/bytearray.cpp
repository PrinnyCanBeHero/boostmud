#include <cstring>

#include "bytearray.hpp"

using namespace boostMUD::util;

ByteArray::ByteArray() : m_size(0) { }
/** @param buffer Character array to use to create the byte array.
 *  @param size Size in bytes of the buffer being passed in. 
 *  @warning The byte array will take ownership of this pointer and will delete it when it leaves scope!
 */
ByteArray::ByteArray(char *buffer, const size_t &size) 
: m_buffer(buffer), m_size(size)
{

}
/** @param buffer Character array to use to create the byte array.
 *  @param size Size in bytes of the buffer being passed in. 
 */
ByteArray::ByteArray(const unsigned char *buffer, const size_t &size) 
: m_buffer(new char[size]), m_size(size)
{
        memcpy(m_buffer.get(), buffer, m_size);
}
/** @param buffer Character array to be copied into the byte array.
 *  @param size Size in bytes of the buffer being passed in. 
 *  @warning The byte array will take ownership of this pointer and will delete it when it leaves scope!
 */
ByteArray::ByteArray(std::unique_ptr<char[]> &buffer, const size_t &size) 
: m_size(size)
{
m_buffer = std::move(buffer);
}
/** @param size Size in bytes of the byte array to be created. */
ByteArray::ByteArray(const size_t &size) 
: m_buffer(new char[size]), m_size(size)
{
}
ByteArray::ByteArray(const ByteArray &rhs) 
: m_buffer(new char[rhs.m_size]), m_size(rhs.m_size)
{
	memcpy(m_buffer.get(), rhs.ptr(), m_size);
}
ByteArray::ByteArray(const ByteArray &left, const ByteArray &right)
: m_buffer(new char[right.m_size+left.m_size]), m_size(right.m_size+left.m_size)
{
	memcpy(m_buffer.get(), left.ptr(), left.size());
	memcpy(m_buffer.get() + left.size(), right.ptr(), right.size());
}
ByteArray::ByteArray(ByteArray &&rhs)
: m_buffer(std::move(rhs.m_buffer)), m_size(std::move(rhs.m_size))
{
  rhs.m_size = 0;

}

ByteArray& ByteArray::operator =( const ByteArray& rhs)
{
  char* tmp = new char[rhs.m_size];
  m_size = rhs.m_size;
  memcpy(tmp, rhs.ptr(), m_size);
  m_buffer.reset(tmp);
  return *this;
}
