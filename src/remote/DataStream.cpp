/*
 * Rvzware based in CAPAWARE 3D
 *
 * Rvzware is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * Rvzware is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The Rvzware development team
 */

#include <string>

#include <remote/stdafx.h>

#include <cpw/common/pstdint.h>
#include <remote/DataStream.h>


using namespace cpw::remote;

bool isBigEndian()
{
	uint32_t tmp = 1;

	if (((uint8_t *) &tmp)[0] == 1)
		return false;
	else
		return true;
}

bool DataStream::big_endian = isBigEndian();

/*!
 *  \brief Default constructor
 *
 *  Sets the stream to a zero-sized stream.
 */
DataStream::DataStream()
{
	data = NULL;
	size = 0;
	current_pos = 0;
}


/*!
 *  \param size The size of the stream
 *  \param stream A pointer to the stream's data
 *
 *  Creates a DataStream object from the data pointed by stream.
 *
 *  Pointer contents are copied to a new structure, so the parameter "stream" can be (and should be) freed.
 *
 *  After creation, internal iterator will point to the begining of the Stream.
 */
DataStream::DataStream(int size, uint8_t *stream)
{
	current_pos = 0;
	this->size = size;
	data = new uint8_t[size];

	memcpy(data, stream, size);
}


/*!
 *  \param size The size of the stream
 *
 *  Creates an empty DataStream of the specified size.
 */
DataStream::DataStream(int size)
{
	current_pos = 0;
	this->size = size;
	data = new uint8_t[size];
}


/*!
 *  \brief Copy constructor
 *  \param stream The stream to be copied
 *
 *  Copy constructor.
 */
DataStream::DataStream(const DataStream &stream)
{
	current_pos = 0;
	size = stream.size;
	data = new uint8_t[size];

	memcpy(data, stream.data, size);
}


DataStream::~DataStream()
{
	if (data != NULL)
		delete [] data;
}


/*!
 *  \brief Assignment operator
 *  \param stream Stream to be copied
 *
 *  Assignment operator.
 */
DataStream &DataStream::operator=(const DataStream &stream)
{
	if (data != NULL)
		delete [] data;

	size = stream.size;
	data = new uint8_t[size];

	current_pos = stream.current_pos;

	for (int i=0; i<size; i++)
		data[i] = stream.data[i];

	return *this;
}


/*!
 *  \brief Returns the size of the stream
 *
 *  Returns the size of the stream.
 */
int DataStream::GetSize() const
{
	return size;
}


/*!
 *  \brief Returns a pointer to the data
 *
 *  Returns a pointer to the data.
 *
 *  <b>WARNING: </b> The pointer will point to an invalid address after the object has been
 *  destroyed. This method is only meant to be used when accessing to the socket library.
 */
uint8_t *DataStream::GetDataPointer() const
{
	return data;
}


/*!
 *  \brief Returns a reference to the "index" position on the stream
 *  \param index Index to the vector
 *  \throws dsExceptOverflow Thrown when the index is out of range
 *
 *  Returns a reference to the "index" position on the stream.
 *
 *  It is considered that each position is a byte in size.
 *
 *  This method is only meant to be used when accessing to the socket library.
 */
uint8_t &DataStream::operator[](const int index) const
{
	if (index < size)
		return data[index];
	else
		throw dsExceptOverflow;
}


/*!
 *  \brief Inserts a value into the stream
 *  \param stream A DataStream object to be inserted into the stream
 *  \throws  dsExceptOverflow Thrown when there is no available space for the data
 *
 *  Inserts a value into the stream.
 *
 *  The data is inserted on the next position available, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator<<.
 *
 *  This overloaded method accepts a DataStream object.
 *
 *  The resulting stream will contain an uint32_t value with the size of the inserted stream preceding
 *  the stream itself.
 */
DataStream &DataStream::operator<<(const DataStream &stream)
{
	(*this) << (uint32_t)stream.size;

	if (current_pos+stream.size-1 >= size)
		throw dsExceptOverflow;

	memcpy(&(data[current_pos]), stream.data, stream.size);
	current_pos += stream.size;

	return *this;
}


/*!
 *  \brief Inserts a value into the stream
 *  \param b The boolean to be inserted into the stream
 *  \throws  dsExceptOverflow Thrown when there is no available space for the data
 *
 *  Inserts a value into the stream.
 *
 *  The data is inserted on the next position available, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator<<.
 *
 *  This overloaded method accepts a boolean data.
 */
DataStream &DataStream::operator<<(bool b)
{
	if (current_pos >= size)
		throw dsExceptOverflow;
	data[current_pos++] = (b?1:0);

	return *this;
}


/*!
 *  \brief Inserts a value into the stream
 *  \param n An 8 bit long unsigned integer to be inserted into the stream
 *  \throws  dsExceptOverflow Thrown when there is no available space for the data
 *
 *  Inserts a value into the stream.
 *
 *  The data is inserted on the next position available, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator<<.
 *
 *  This overloaded method accepts an 8 bit long unsigned integer.
 */
DataStream &DataStream::operator<<(uint8_t n)
{
	if (current_pos >= size)
		throw dsExceptOverflow;
	data[current_pos++] = n;

	return *this;
}


/*!
 *  \brief Inserts a value into the stream
 *  \param n An 32 bit long unsigned integer to be inserted into the stream
 *  \throws  dsExceptOverflow Thrown when there is no available space for the data
 *
 *  Inserts a value into the stream.
 *
 *  The data is inserted on the next position available, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator<<.
 *
 *  Integer data is converted into network byte order before its insertion.
 *
 *  This overloaded method accepts an 32 bit long unsigned integer.
 */
DataStream &DataStream::operator<<(uint32_t n)
{
	if (current_pos+3 >= size)
		throw dsExceptOverflow;

	n = DataStream::ToNetworku32(n);
	data[current_pos++] = ((uint8_t*) &n)[0];
	data[current_pos++] = ((uint8_t*) &n)[1];
	data[current_pos++] = ((uint8_t*) &n)[2];
	data[current_pos++] = ((uint8_t*) &n)[3];

	return *this;
}


/*!
 *  \brief Inserts a value into the stream
 *  \param n An 64 bit long unsigned integer to be inserted into the stream
 *  \throws  dsExceptOverflow Thrown when there is no available space for the data
 *
 *  Inserts a value into the stream.
 *
 *  The data is inserted on the next position available, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator<<.
 *
 *  Integer data is converted into network byte order before its insertion.
 *
 *  This overloaded method accepts an 64 bit long unsigned integer.
 */
DataStream &DataStream::operator<<(uint64_t n)
{
	if (current_pos+7 >= size)
		throw dsExceptOverflow;
	n = DataStream::ToNetworku64(n);
	data[current_pos++] = ((uint8_t*) &n)[0];
	data[current_pos++] = ((uint8_t*) &n)[1];
	data[current_pos++] = ((uint8_t*) &n)[2];
	data[current_pos++] = ((uint8_t*) &n)[3];
	data[current_pos++] = ((uint8_t*) &n)[4];
	data[current_pos++] = ((uint8_t*) &n)[5];
	data[current_pos++] = ((uint8_t*) &n)[6];
	data[current_pos++] = ((uint8_t*) &n)[7];

	return *this;
}


/*!
 *  \brief Inserts a value into the stream
 *  \param s A string to be inserted into the stream
 *  \throws  dsExceptOverflow Thrown when there is no available space for the data
 *
 *  Inserts a value into the stream.
 *
 *  The data is inserted on the next position available, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator<<.
 *
 *  This overloaded method accepts a string.
 *
 *  The resulting stream will contain an uint32_t value with the size of the string preceding the
 *  characters of the string.
 */
DataStream &DataStream::operator<<(const std::string &s)
{
	if (current_pos+s.size()+4-1 >= (unsigned)size)
		throw dsExceptOverflow;

	(*this) << (uint32_t)s.size();

	std::string::const_iterator it_str;
	for (it_str = s.begin(); it_str != s.end(); it_str++)
		data[current_pos++] = *it_str;

	return *this;
}


/*!
 *  \brief Inserts a value into the stream
 *  \param id An entity's id to be inserted into the stream
 *  \throws  dsExceptOverflow Thrown when there is no available space for the data
 *
 *  Inserts a value into the stream.
 *
 *  The data is inserted on the next position available, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator<<.
 *
 *  This overloaded method accepts a TypeId.
 */
DataStream &DataStream::operator<<(const cpw::TypeId &id)
{
	if (current_pos+cpw::TypeId::size() -1 >= size)
		throw dsExceptOverflow;

	cpw::TypeId idtemp = id;
	for (int i=0; i<cpw::TypeId::size(); i++)
		data[current_pos++] = idtemp[i];

	return *this;
}


/*!
 *  \brief Extracts a value from the stream
 *  \param stream A DataStream to hold the one extrated from the stream
 *  \throws  dsExceptOverflow Thrown when the size of the remaining stream is not enough to hold the data
 *
 *  Extracts a value from the stream.
 *
 *  The data is extracted from the next position, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator>>.
 *
 *  This overloaded method accepts a DataStream object.
 *
 *  The size of the stream to be extracted is embeded into the stream, followed by the stream itself, as 
 *  the opposite method operator<< did.
 */
DataStream &DataStream::operator>>(DataStream &stream)
{
	uint32_t opsize;
	(*this) >> opsize;

	if (current_pos+opsize-1 >= size)
		throw dsExceptOverflow;

	stream = DataStream(opsize, &(data[current_pos]));
	current_pos += opsize;

	return *this;
}


/*!
 *  \brief Extracts a value from the stream
 *  \param b A boolean to hold the one extrated from the stream
 *  \throws dsExceptOverflow Thrown when the size of the remaining stream is not enough to hold the data
 *
 *  Extracts a value from the stream.
 *
 *  The data is extracted from the next position, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator>>.
 *
 *  This overloaded method accepts a 64 bit long unsigned integer.
 */
DataStream &DataStream::operator>>(bool &b)
{
	if (current_pos >= size)
		throw dsExceptOverflow;
	data[current_pos++] ? b=true : b=false;
	return *this;
}


/*!
 *  \brief Extracts a value from the stream
 *  \param n An 8 bit long unsigned integer to hold the one extrated from the stream
 *  \throws dsExceptOverflow Thrown when the size of the remaining stream is not enough to hold the data
 *
 *  Extracts a value from the stream.
 *
 *  The data is extracted from the next position, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator>>.
 *
 *  This overloaded method accepts a 8 bit long unsigned integer.
 */
DataStream &DataStream::operator>>(uint8_t &n)
{
	if (current_pos >= size)
		throw dsExceptOverflow;
	n = data[current_pos++];
	return *this;
}


/*!
 *  \brief Extracts a value from the stream
 *  \param n A 32 bit long unsigned integer to hold the one extrated from the stream
 *  \throws dsExceptOverflow Thrown when the size of the remaining stream is not enough to hold the data
 *
 *  Extracts a value from the stream.
 *
 *  The data is extracted from the next position, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator>>.
 *
 *  This overloaded method accepts a 32 bit long unsigned integer.
 */
DataStream &DataStream::operator>>(uint32_t &n)
{
	if (current_pos+3 >= size)
		throw dsExceptOverflow;
	uint32_t tmp;
	((uint8_t*)&tmp)[0] = data[current_pos++];
	((uint8_t*)&tmp)[1] = data[current_pos++];
	((uint8_t*)&tmp)[2] = data[current_pos++];
	((uint8_t*)&tmp)[3] = data[current_pos++];
	n = DataStream::FromNetworku32(tmp);

	return *this;
}


/*!
 *  \brief Extracts a value from the stream
 *  \param n A 64 bit long unsigned integer to hold the one extrated from the stream
 *  \throws dsExceptOverflow Thrown when the size of the remaining stream is not enough to hold the data
 *
 *  Extracts a value from the stream.
 *
 *  The data is extracted from the next position, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator>>.
 *
 *  This overloaded method accepts a 64 bit long unsigned integer.
 */
DataStream &DataStream::operator>>(uint64_t &n)
{
	if (current_pos+7 >= size)
		throw dsExceptOverflow;
	uint64_t tmp;
	((uint8_t*)&tmp)[0] = data[current_pos++];
	((uint8_t*)&tmp)[1] = data[current_pos++];
	((uint8_t*)&tmp)[2] = data[current_pos++];
	((uint8_t*)&tmp)[3] = data[current_pos++];
	((uint8_t*)&tmp)[4] = data[current_pos++];
	((uint8_t*)&tmp)[5] = data[current_pos++];
	((uint8_t*)&tmp)[6] = data[current_pos++];
	((uint8_t*)&tmp)[7] = data[current_pos++];
	n = DataStream::FromNetworku64(tmp);

	return *this;
}


/*!
 *  \brief Extracts a value from the stream
 *  \param s A string to hold the one extrated from the stream
 *  \throws dsExceptOverflow Thrown when the size of the remaining stream is not enough to hold the data
 *
 *  Extracts a value from the stream.
 *
 *  The data is extracted from the next position, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator>>.
 *
 *  This overloaded method accepts a string.
 *
 *  The size of the string to be extracted is embeded into the stream, followed by the string itself, as 
 *  the opposite method operator<< did.
 */
DataStream &DataStream::operator>>(std::string &s)
{
	s = std::string();
	uint32_t s_size;
	(*this)>>s_size;
	if (current_pos+s_size-1 >= (unsigned)size)
		throw dsExceptOverflow;
	while ((unsigned)s_size > 0)
	{
		s += (unsigned char) data[current_pos++];
		s_size--;
	}

	return *this;
}


/*!
 *  \brief Extracts a value from the stream
 *  \param id An entity's id object to hold the one extrated from the stream
 *  \throws dsExceptOverflow Thrown when the size of the remaining stream is not enough to hold the data
 *
 *  Extracts a value from the stream.
 *
 *  The data is extracted from the next position, according to an internal iterator that begins 
 *  on position 0 of the stream, and is incremented with every call to operator>>.
 *
 *  This overloaded method accepts a TypeId.
 */
DataStream &DataStream::operator>>(cpw::TypeId &id)
{
	if (current_pos+cpw::TypeId::size() -1 >= size)
		throw dsExceptOverflow;
	for (int i=0; i<cpw::TypeId::size(); i++)
		id[i] = data[current_pos++];

	return *this;
}


/*!
 *  \brief Returns the parameter transformed into network byte order
 *  \param number The value to be converted
 *
 *  Returns the parameters transformed into network byte order.
 */
uint32_t DataStream::ToNetworku32(uint32_t number)
{
	if (!big_endian)
	{
		uint32_t new_number;
		((uint8_t*) &new_number)[3] = ((uint8_t*) &number)[0];
		((uint8_t*) &new_number)[2] = ((uint8_t*) &number)[1];
		((uint8_t*) &new_number)[1] = ((uint8_t*) &number)[2];
		((uint8_t*) &new_number)[0] = ((uint8_t*) &number)[3];
		return new_number;
	}
	return number;
}


/*!
 *  \brief Returns the parameter transformed into network byte order
 *  \param number The value to be converted
 *
 *  Returns the parameters transformed into network byte order.
 */
uint64_t DataStream::ToNetworku64(uint64_t number)
{
	if (!big_endian)
	{
		uint64_t new_number;
		((uint8_t*) &new_number)[7] = ((uint8_t*) &number)[0];
		((uint8_t*) &new_number)[6] = ((uint8_t*) &number)[1];
		((uint8_t*) &new_number)[5] = ((uint8_t*) &number)[2];
		((uint8_t*) &new_number)[4] = ((uint8_t*) &number)[3];
		((uint8_t*) &new_number)[3] = ((uint8_t*) &number)[4];
		((uint8_t*) &new_number)[2] = ((uint8_t*) &number)[5];
		((uint8_t*) &new_number)[1] = ((uint8_t*) &number)[6];
		((uint8_t*) &new_number)[0] = ((uint8_t*) &number)[7];
		return new_number;
	}
	return number;
}


/*!
 *  \brief Returns the parameter transformed into local byte order from network byte order
 *  \param number The value to be converted
 *
 *  Returns the parameters transformed into local byte order from network byte order.
 */
uint32_t DataStream::FromNetworku32(uint32_t number)
{
	if (!big_endian)
	{
		uint32_t new_number;
		((uint8_t*) &new_number)[3] = ((uint8_t*) &number)[0];
		((uint8_t*) &new_number)[2] = ((uint8_t*) &number)[1];
		((uint8_t*) &new_number)[1] = ((uint8_t*) &number)[2];
		((uint8_t*) &new_number)[0] = ((uint8_t*) &number)[3];
		return new_number;
	}
	return number;
}


/*!
 *  \brief Returns the parameter transformed into local byte order from network byte order
 *  \param number The value to be converted
 *
 *  Returns the parameters transformed into local byte order from network byte order.
 */
uint64_t DataStream::FromNetworku64(uint64_t number)
{
	if (!big_endian)
	{
		uint64_t new_number;
		((uint8_t*) &new_number)[7] = ((uint8_t*) &number)[0];
		((uint8_t*) &new_number)[6] = ((uint8_t*) &number)[1];
		((uint8_t*) &new_number)[5] = ((uint8_t*) &number)[2];
		((uint8_t*) &new_number)[4] = ((uint8_t*) &number)[3];
		((uint8_t*) &new_number)[3] = ((uint8_t*) &number)[4];
		((uint8_t*) &new_number)[2] = ((uint8_t*) &number)[5];
		((uint8_t*) &new_number)[1] = ((uint8_t*) &number)[6];
		((uint8_t*) &new_number)[0] = ((uint8_t*) &number)[7];
		return new_number;
	}
	return number;
}
