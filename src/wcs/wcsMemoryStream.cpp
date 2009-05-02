/** THIS IS NOT THE ORIGINAL FILE.
 * THIS FILE HAS BEEN MODIFIED BY MANUEL PADRON MARTINEZ FOR RVZWARE
 * AND PREVIOUSLY BY CAPAWARE TEAM FOR CAPAWARE
 */
/* -*-c++-*- libwms - Copyright (C) since 2004 Garrett Potts 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the libwms Public License (WMSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * libwms Public License for more details.
 */
#include <wcs/wcsMemoryStream.h>
#include <iostream>
#include <iterator>
#include <string.h>

wcsMemoryBufferStream::wcsMemoryBufferStream()
{
  the_buf_size     = 0;
  the_buf          = 0;
  the_buf_size_opt = the_buf_size;
  the_in_beg       = the_buf;
  the_in_end       = the_buf;
  the_out_beg      = the_buf;
  the_out_end      = the_in_end;
  the_in_cur       = the_buf;
  the_out_cur      = the_buf;
}

wcsMemoryBufferStream::~wcsMemoryBufferStream()
{
  clear();
}

int wcsMemoryBufferStream::overflow(int c)
{
  extendBuffer(1);
  *the_out_cur = (char_type)c;
  the_out_cur += 1;
   
  return c;
}

std::streamsize wcsMemoryBufferStream::xsputn(const char * pChar, std::streamsize n)
{
  if(n == 0) return 0;

  std::streampos r1 = range(the_out_beg, the_out_end);
  std::streampos r2 = range(the_out_cur, the_out_beg);

  if((r2 - r1) < n)
    {
      extendBuffer(n - (r2-r1));
    }
   
  memcpy(the_out_cur, pChar, n);
   
  the_out_cur += n;
   
  return n;
}

std::streamsize wcsMemoryBufferStream::xsgetn(char_type* __s, std::streamsize n)
{
  std::streampos r1 = range(the_in_cur, the_in_end);

  std::streamsize bytesToRead = n;
  if(r1 < n)
    {
      bytesToRead = r1;
    }
  memcpy(__s, the_in_cur, bytesToRead);
   
  the_in_cur += bytesToRead;

  if(bytesToRead > 0)
    {
      return n;
    }
  return 0;
}

wcsMemoryBufferStream::pos_type wcsMemoryBufferStream::seekoff(off_type /*t*/,
                                                               std::ios_base::seekdir dir,
                                                               std::ios_base::openmode omode )
{
  off_type offsetType = -1;
  char_type *beg = 0;
  char_type *end = 0;
  char_type *cur = 0;
  char_type *buf = the_buf;
  if(omode == std::ios_base::in)
    {
      beg = the_in_beg;
      end = the_in_end;
      cur = the_in_cur;
    }
  else if(omode == std::ios_base::out)
    {
      beg = the_out_beg;
      end = the_out_end;
      cur = the_out_cur;
    }
  switch(dir)
    {
    case std::ios_base::cur:
      {
	offsetType =range(buf,cur);
	break;
      }
    case std::ios_base::beg:
      {
	offsetType =range(buf, beg);
	break;
      }
    case std::ios_base::end:
      {
	offsetType =range(buf, end);
	break;
      }
    default:
      {
	// error
      }
    }
   
  return pos_type(offsetType);
}

wcsMemoryBufferStream::pos_type wcsMemoryBufferStream::seekpos(pos_type posType, 
							       std::ios_base::openmode omode)
{
  pos_type pType = -1;
  if(omode == std::ios_base::in)
    {
      if(posType < range(the_in_beg, the_in_end))
	{
	  the_in_cur = the_in_beg + posType;
	  pType = posType;
	}
    }
  else if(omode == std::ios_base::out)
    {
      if(posType < range(the_out_beg, the_out_end))
	{
	  the_out_cur = the_out_beg + posType;
	  pType = posType;
	}
    }
   
  return pType;
}


void wcsMemoryBufferStream::extendBuffer(std::streamsize n)
{
  if(the_buf_size == 0)
    {
      the_buf          = new char_type[n];
      the_buf_size     = n;
      the_buf_size_opt = the_buf_size;
      the_in_beg       = the_buf;
      the_in_end       = the_buf + n;
      the_out_beg      = the_buf;
      the_out_end      = the_in_end;
      the_in_cur       = the_buf;
      the_out_cur      = the_buf;
    }
  else
    {
      char_type* tempBuf = new char_type[the_buf_size + n];
      memcpy(tempBuf, the_buf, the_buf_size);

      std::streamsize offin = range(the_in_cur, the_in_beg);
      std::streamsize offout = range(the_out_cur, the_out_beg);
      if (the_buf)
	{
	  delete [] the_buf;
	  the_buf = NULL;
	}
      the_buf = tempBuf;
      the_buf_size     += n;
      the_buf_size_opt = the_buf_size;
      the_in_beg       = the_buf;
      the_in_end       = the_buf + the_buf_size;
      the_out_beg      = the_buf;
      the_out_end      = the_in_end;
      
      the_in_cur       = the_buf + offin;
      the_out_cur      = the_buf + offout;
    }
}

wcsMemoryBufferStream::off_type wcsMemoryBufferStream::range(char_type* buf1,
							     char_type* buf2)const
{
  if(buf1 < buf2)
    {
      return (wcsMemoryBufferStream::off_type)(buf2-buf1);
    }
   
  return (wcsMemoryBufferStream::off_type)(buf1-buf2);
}

wcsMemoryBufferStream::int_type wcsMemoryBufferStream::underflow()
{
  return traits_type::eof();
}

void wcsMemoryBufferStream::clear()
{
  if(the_buf)
    {
      delete [] the_buf;
      the_buf = 0;
    }
  the_buf_size     = 0;
  the_buf          = 0;
  the_buf_size_opt = the_buf_size;
  the_in_beg       = the_buf;
  the_in_end       = the_buf;
  the_out_beg      = the_buf;
  the_out_end      = the_in_end;
  the_in_cur       = the_buf;
  the_out_cur      = the_buf;
}

std::streamsize wcsMemoryBufferStream::getBufferSize()const
{
  return (std::streamsize)the_buf_size;
}

const wcsMemoryBufferStream::char_type* wcsMemoryBufferStream::getBuffer()const
{
  return the_buf;
}

wcsMemoryBufferStream::char_type* wcsMemoryBufferStream::getBuffer()
{
  return the_buf;
}

std::streamsize wcsMemoryStream::getBufferSize()const
{
  return theMemoryBufferStream.getBufferSize();
}

const wcsMemoryBufferStream::char_type* wcsMemoryStream::getBuffer()const
{
  return theMemoryBufferStream.getBuffer();
}

wcsMemoryBufferStream::char_type* wcsMemoryStream::getBuffer()
{
  return theMemoryBufferStream.getBuffer();
}

void wcsMemoryStream::clear()
{
  theMemoryBufferStream.clear();
}

wcsMemoryStream::wcsMemoryStream(const char* buf, std::streamsize size)
  : std::ios(0), std::ostream(&theMemoryBufferStream), std::istream(&theMemoryBufferStream)
{
  if(buf)
    {
      write(buf, size);
    }
}
std::string wcsMemoryStream::getBufferAsString()const
{
  if(getBuffer())
    {
      return std::string(getBuffer(),
                         getBuffer() + getBufferSize());
    }

  return std::string("");
}

