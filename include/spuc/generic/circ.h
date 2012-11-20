/*
 * SPUC - Signal processing using C++ - A DSP library
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef CIRCB
#define CIRCB
namespace SPUC {
//! \ingroup miscfunc
//! \brief Circular Buffer
//!  \author Tony Kirke,  Copyright(c) 2001 
// Circular buffer class
template <class T> class circ_buffer {

 protected:
	int	len;
	int	ptr;
	T 	*buf;

 public:
	circ_buffer(void) : buf(NULL), len(0), ptr(0) { ; }
	circ_buffer(const circ_buffer<T>& A);
	circ_buffer(int d);
	circ_buffer(int d, T init_value);
	~circ_buffer(void) {
		delete[] buf;
	}

	int size(void) const {return len;};

	T operator [](int i) const {
		return buf[(ptr+i)%len];
	};
	circ_buffer<T> circ_buffer::operator =(circ_buffer<T>& A);
	void put(T data_in) {
		ptr = (ptr-1+len)%len;
		buf[ptr] = data_in;
	}
};

template <class T> circ_buffer<T>::circ_buffer(const circ_buffer<T>& A)
{
	len = A.len;
	ptr = A.ptr;
	buf = new T[len];
	for (int i = 0; i < len; i++) buf[i] = A.buf[i];

}	
// copy constructor
template <class T> circ_buffer<T>::circ_buffer(int len1)
{
	len = len1;
	ptr = len-1;
	buf = new T[len];
}

template <class T> circ_buffer<T>::circ_buffer(int len1, T init_value)
{
	circ_buffer(len1);
	for (int i= 0; i < len; i++)	buf[i] = init_value;
}

template <class T> circ_buffer<T> circ_buffer<T>::operator =(circ_buffer<T>& A)
{
	if (this->len != A.size())	{
		// remove existing circ
		delete[] buf;
		// create room for A
		len = A.size();
		buf = new T[len];
	}
	ptr = A.ptr;

	for (int i = 0; i < len; i++)	buf[i] = A.buf[i];
	return *this;
}
}
#endif




