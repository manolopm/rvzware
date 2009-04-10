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
#ifndef wcsMemoryStream_HEADER
#define wcsMemoryStream_HEADER
#include <iostream>
#include <string>
#include <streambuf>
#include <wcs/wcsReferenced.h>

class WCS_EXPORT wcsMemoryBufferStream : public std::streambuf
{
public:
   friend class wcsMemoryStream;
   
   wcsMemoryBufferStream();
   virtual ~wcsMemoryBufferStream();
   virtual int overflow(int c);
   virtual std::streamsize xsputn(const char * pChar, std::streamsize n);
   virtual std::streamsize xsgetn(char_type* __s, std::streamsize n);
   virtual pos_type seekoff(off_type t, std::ios_base::seekdir dir,
                            std::ios_base::openmode omode = std::ios_base::in |
                            std::ios_base::out);
   virtual pos_type seekpos(pos_type posType, 
                            std::ios_base::openmode /*__mode*/ = std::ios_base::in | std::ios_base::out);
   std::streamsize getBufferSize()const;
   const char_type* getBuffer()const;
   char_type* getBuffer();
   virtual int_type underflow();
   void clear();
protected:
   void extendBuffer(std::streamsize n);
   off_type range(char_type* buf1,
                  char_type* buf2)const;
      char_type*		the_buf; 	

      /**
       *  @if maint
       *  Actual size of allocated internal buffer, in bytes.
       *  @endif
      */
      size_t			the_buf_size;

      /**
       *  @if maint
       *  Optimal or preferred size of internal buffer, in bytes.
       *  @endif
      */
      size_t			the_buf_size_opt;


      //@{
      /**
       *  @if maint
       *  This is based on _IO_FILE, just reordered to be more consistent,
       *  and is intended to be the most minimal abstraction for an
       *  internal buffer.
       *  -  get == input == read
       *  -  put == output == write
       *  @endif
      */
      char_type* 		the_in_beg;  	// Start of get area. 
      char_type* 		the_in_cur;	// Current read area. 
      char_type* 		the_in_end;	// End of get area. 
      char_type* 		the_out_beg; 	// Start of put area. 
      char_type* 		the_out_cur;  	// Current put area. 
      char_type* 		the_out_end;  	// End of put area. 
};

class WCS_EXPORT wcsMemoryStream : public wcsReferenced, public std::ostream, public std::istream
{
public: 
   wcsMemoryStream() : std::ios(0), std::ostream(&theMemoryBufferStream), std::istream(&theMemoryBufferStream){}
   virtual ~wcsMemoryStream() 
      {
         theMemoryBufferStream.pubsync(); 
      }
   wcsMemoryStream(const char* bufferToCopy, std::streamsize size);
   
   std::streamsize getBufferSize()const;
   const wcsMemoryBufferStream::char_type* getBuffer()const;
   wcsMemoryBufferStream::char_type* getBuffer();
   std::string getBufferAsString()const;
   
   void clear();
private:
   // Copy & assignment are undefined in iostreams
   wcsMemoryStream & operator=(const wcsMemoryStream&){return *this;}
   wcsMemoryStream(const wcsMemoryStream& src) : std::ios(0), wcsReferenced(src), std::ostream(&theMemoryBufferStream), std::istream(&theMemoryBufferStream){}

   
   wcsMemoryBufferStream theMemoryBufferStream; 
};

#endif
