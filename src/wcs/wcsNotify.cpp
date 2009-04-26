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
#include <wcs/wcsNotify.h>
#include <stdio.h>
#include <stdarg.h>
#include <stack>
#include <fstream>

static std::ostream* theWcsFatalStream  = &std::cerr;
static std::ostream* theWcsWarnStream   = &std::cerr;
static std::ostream* theWcsInfoStream   = &std::cout;
static std::ostream* theWcsNoticeStream = &std::cout;
static std::ostream* theWcsDebugStream  = &std::cout;
static std::ostream* theWcsAlwaysStream = &std::cout;

static wcsNotifyFlags theNotifyFlags  = wcsNotifyFlags_ALL;
static std::stack<wcsNotifyFlags> theNotifyFlagsStack;

template <class charT, class traits = std::char_traits<charT> >
class wcsNullBufferStream : public std::basic_streambuf<charT, traits>
{
public:
   wcsNullBufferStream(){};
   
   
protected:
   
   std::streamsize xsputn(const charT * /*pChar*/, std::streamsize /*n*/)
      {
         return 0;
      }
   
private:
   wcsNullBufferStream(const wcsNullBufferStream&);
   wcsNullBufferStream& operator=(const wcsNullBufferStream&);
};

template <class charT, class traits = std::char_traits<charT> >
class wcsLogFileBufferStream : public std::basic_streambuf<charT, traits>
{
public:
   wcsLogFileBufferStream(){};
   
   void setLogFilename(const std::string& file)
      {
         theLogFilename = file;
      }
   std::string getLogFilename()const
      {
         return theLogFilename;
      }
   
protected:
   std::string theLogFilename;
   virtual int overflow(int c)
      {
         if(!traits::eq_int_type(c, traits::eof()))
         {
            char tempStr[2];
            tempStr[0] = c;
            tempStr[1] = '\0';
            
            tempString = tempString + tempStr;
         }
         
         return c;
      }
   
   virtual std::streamsize xsputn(const charT * pChar, std::streamsize n)
      {
         tempString = tempString + std::string(pChar, pChar + n);

         return n;
      }

   virtual int sync()
      {
         if ( theLogFilename.size() && tempString.size() )
         {
            std::ofstream outFile( theLogFilename.c_str(),
                                   std::ios::app|std::ios::out );
            if (outFile.good())
            {
               outFile.write(tempString.c_str(), tempString.length());
               tempString = "";
            }
         }
         return 0;
      }
   
private:
   std::string tempString;
   
   wcsLogFileBufferStream(const wcsLogFileBufferStream&);
   wcsLogFileBufferStream& operator=(const wcsLogFileBufferStream&);
};


class wcsNullStream : public std::ostream
{
public: 
   wcsNullStream() : std::ios(0),std::ostream(&nullBufferStream){}
   virtual ~wcsNullStream() 
      {
         nullBufferStream.pubsync(); 
      }
   
private:
   wcsNullBufferStream<char> nullBufferStream; 
   // Copy & assignment are undefined in iostreams
   wcsNullStream(const wcsNullStream&);
   wcsNullStream & operator=(const wcsNullStream&);
};

class wcsLogFileStream : public std::ostream
{
public: 
   wcsLogFileStream() : std::ios(0),std::ostream(&theLogFileBufferStream){}
   virtual ~wcsLogFileStream() 
      {
         theLogFileBufferStream.pubsync(); 
      }
   void setLogFilename(const std::string& filename)
      {
         theLogFileBufferStream.setLogFilename(filename);
      }
   std::string getLogFilename()const
      {
         return theLogFileBufferStream.getLogFilename();
      }
   
private:
   wcsLogFileBufferStream<char> theLogFileBufferStream; 
   // Copy & assignment are undefined in iostreams
   wcsLogFileStream(const wcsLogFileStream&);
   wcsLogFileStream & operator=(const wcsLogFileStream&);
};

static wcsNullStream    theWcsNullStream;
static wcsLogFileStream theLogFileStream;

void wcsSetDefaultNotifyHandlers()
{
   theWcsFatalStream  = &std::cerr;
   theWcsWarnStream   = &std::cout;
   theWcsInfoStream   = &std::cout;
   theWcsNoticeStream = &std::cout;
   theWcsDebugStream  = &std::cout;
   theWcsAlwaysStream = &std::cout;
}

void wcsSetNotifyStream(std::ostream* outputStream,
                          wcsNotifyFlags whichLevelsToRedirect)
{
   if(whichLevelsToRedirect&wcsNotifyFlags_FATAL)
   {
      theWcsFatalStream = outputStream;
   }
   if(whichLevelsToRedirect&wcsNotifyFlags_WARN)
   {
      theWcsWarnStream = outputStream;
   }
   if(whichLevelsToRedirect&wcsNotifyFlags_INFO)
   {
      theWcsInfoStream = outputStream;
   }
   if(whichLevelsToRedirect&wcsNotifyFlags_NOTICE)
   {
      theWcsNoticeStream = outputStream;
   }
   if(whichLevelsToRedirect&wcsNotifyFlags_DEBUG)
   {
      theWcsDebugStream = outputStream;
   }
}

std::ostream* wcsGetNotifyStream(wcsNotifyLevel whichLevel)
{
   std::ostream* notifyStream = &theWcsNullStream;

   switch(whichLevel)
   {
      case wcsNotifyLevel_ALWAYS:
      {
         notifyStream = theWcsAlwaysStream;
         break;
      }
      case wcsNotifyLevel_FATAL:
      {
         notifyStream = theWcsFatalStream;
         break;
      }
      case wcsNotifyLevel_WARN:
      {
         notifyStream = theWcsWarnStream;
         break;
      }
      case wcsNotifyLevel_INFO:
      {
         notifyStream = theWcsInfoStream;
         break;
      }
      case wcsNotifyLevel_NOTICE:
      {
         notifyStream = theWcsNoticeStream;
         break;
      }
      case wcsNotifyLevel_DEBUG:
      {
         notifyStream = theWcsDebugStream;
         break;
      }
   }
   return notifyStream;
}

std::ostream& wcsNotify(wcsNotifyLevel level)
{
   if(wcsIsNotifyEnabled())
   {
      if(theLogFileStream.getLogFilename() != "")
      {
         return theLogFileStream;
      }
      else
      {
         bool reportMessageFlag = false;
         switch(level)
         {
            case wcsNotifyLevel_ALWAYS:
            {
               reportMessageFlag = true;
               break;
            }
            case wcsNotifyLevel_FATAL:
            {
               if(theNotifyFlags&wcsNotifyFlags_FATAL)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case wcsNotifyLevel_WARN:
            {
               if(theNotifyFlags&wcsNotifyFlags_WARN)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case wcsNotifyLevel_INFO:
            {
               if(theNotifyFlags&wcsNotifyFlags_INFO)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case wcsNotifyLevel_NOTICE:
            {
               if(theNotifyFlags&wcsNotifyFlags_NOTICE)
               {
                  reportMessageFlag = true;
               }
               break;
            }
            case wcsNotifyLevel_DEBUG:
            {
               if(theNotifyFlags&wcsNotifyFlags_DEBUG)
               {
                  reportMessageFlag = true;
               }
               break;
            }
         }
         if(reportMessageFlag)
         {
            return *wcsGetNotifyStream(level);
         }
      }
   }

   return theWcsNullStream;
}

void wcsSetLogFilename(const std::string& filename)
{
   theLogFileStream.setLogFilename(filename);
}

const char* wcsErrorV(const char *fmt, va_list args )
{  
   static char temp[2024];
   if(fmt)
   {
      vsprintf(temp, fmt, args);
   }
   else
   {
      sprintf(temp,"%s", "");
   }

   return temp;
}

void wcsEnableNotify(wcsNotifyFlags flags)
{
   theNotifyFlags = (wcsNotifyFlags)(theNotifyFlags | flags);
}

void wcsDisableNotify(wcsNotifyFlags flags)
{
   theNotifyFlags = (wcsNotifyFlags)((wcsNotifyFlags_ALL^flags)&
                                       theNotifyFlags);
}

void wcsSetNotifyFlag(wcsNotifyFlags notifyFlags)
{
   theNotifyFlags = notifyFlags;
}

void wcsPushNotifyFlags()
{
   theNotifyFlagsStack.push(theNotifyFlags);
}

void wcsPopNotifyFlags()
{
   if(theNotifyFlagsStack.empty())
   {
      return;
   }
   theNotifyFlags = theNotifyFlagsStack.top();
   theNotifyFlagsStack.pop();
}

wcsNotifyFlags wcsGetNotifyFlags()
{
   return theNotifyFlags;
}



bool wcsIsNotifyEnabled()
{
   return  (theNotifyFlags != wcsNotifyFlags_NONE);
}


