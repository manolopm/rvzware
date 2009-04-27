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

#ifndef _DATASTREAM_
#define _DATASTREAM_

#include <string>

#include <cpw/common/pstdint.h>
#include <cpw/common/TypeId.h>
#include <remote/RemoteExport.h>

/*!
 *  \file DataStream.h
 */

namespace cpw 
{ 
	namespace remote
	{
		/*!
		 *  \ingroup remote
		 *
		 *  Exception codes
		 */
		enum DataStreamException {
			dsExceptOverflow  /*!< Overflow exception */
		};


		/*!
		 *  \class DataStream remote/messages/DataStream.h
		 *  \brief Class containing a message as a secuence of bytes
		 *  \ingroup remote
		 *
		 *  Class containing a message as a secuence of bytes.
		 *
		 *  This class is used to transform messages into byte streams that can be sent through a socket.
		 *
		 *  Use of overloaded operators "operator<<" and "operator>>" is recomended over direct access using
		 *  "operator[]". Use of "operator[]" is meant for special cases.
		 */
		class REMOTE_EXPORT DataStream
		{
		public:
			DataStream();
			DataStream(int _size, uint8_t *_stream);
			DataStream(int _size);
			DataStream(const DataStream &stream);
			~DataStream();

			DataStream &operator=(const DataStream &stream);

			int GetSize() const;
			uint8_t *GetDataPointer() const;

			uint8_t &operator[](const int index) const;

			DataStream &operator<<(const DataStream &operand2);
			DataStream &operator<<(bool b);
			DataStream &operator<<(uint8_t n);
			DataStream &operator<<(uint32_t n);
			DataStream &operator<<(uint64_t n);
			DataStream &operator<<(const std::string &s);
			DataStream &operator<<(const cpw::TypeId &id);

			DataStream &operator>>(DataStream &operand2);
			DataStream &operator>>(bool &b);
			DataStream &operator>>(uint8_t &n);
			DataStream &operator>>(uint32_t &n);
			DataStream &operator>>(uint64_t &n);
			DataStream &operator>>(std::string &s);
			DataStream &operator>>(cpw::TypeId &id);

			static uint32_t ToNetworku32(uint32_t number);
			static uint64_t ToNetworku64(uint64_t number);
			static uint32_t FromNetworku32(uint32_t number);
			static uint64_t FromNetworku64(uint64_t number);

		private:
			uint8_t *data;
			int size;
			int current_pos;
			static bool big_endian;
		};
	}
}

#endif
