#ifndef __BYTE_ARRAY_H_
#define __BYTE_ARRAY_H_

#include <memory>

namespace boostMUD {
/** Namespace for Utility classes. */
namespace util {

/** Class for moving around binary blobs of data. */
class ByteArray {

	public:
		/** Constructs a zero size array. */
		ByteArray();
		/** Constructs an array from a character buffer of a specified size. */
		ByteArray(char *buffer, const size_t &size);
		/** Constructs an array as a copy of a character buffer of a specified size. */
		ByteArray(const unsigned char* buffer, const size_t &size);
		/** Constructs an array from a character buffer of a specified size. */
		ByteArray(std::unique_ptr<char[]> &buffer, const size_t &size);
		/** Constructs an array of the specified size. */
		ByteArray(const size_t &size);
		/** Deep copy constructor. */
		ByteArray(const ByteArray &rhs);
		/** Move constructor. */
		ByteArray(ByteArray &&rhs);
		/** Merge constructor */
		ByteArray(const ByteArray &left, const ByteArray &right);

		ByteArray& operator =( const ByteArray& rhs);

		/** Cast to character pointer. */
		inline operator char*() { return m_buffer.get(); }
		/** Returns pointer to internal data. */
		inline char* ptr() { return m_buffer.get(); }
		/** Returns pointer to internal data. */
		inline const char* ptr() const { return m_buffer.get(); }
		/** Returns pointer to internal data. */
		inline char* data() { return m_buffer.get(); }
		/** Returns size of array. */
		inline const size_t& size() const { return m_size; }
        /** Clears the array */
        inline void clear() { m_size = 0; m_buffer.reset(); }

	private:
		std::unique_ptr<char[]> m_buffer;
		size_t m_size;

};

}

}

#endif
