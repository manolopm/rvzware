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
#include <wcs/wcsCurlMemoryStream.h>
#include <wcs/wcsNotify.h>
#include <fstream>
#include <sstream>

struct wcsCurlMemoryStreamStruct
{
   wcsCurlMemoryStreamStruct()
      {
         curlStream = 0;
      }
   std::ostream* curlStream;
};

int wcsCurlWrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  int rc = 0;

  wcsCurlMemoryStreamStruct *out=(struct wcsCurlMemoryStreamStruct *)stream;

//   if(out->curlStream->getStream()->good())
//   {
//      out->curlStream->getStream()->write((char*)buffer, nmemb*size);
//      rc = nmemb*size;
//   }

   if(out->curlStream&&out->curlStream->good())
   {
      out->curlStream->write((char*)buffer, nmemb*size);
      rc = nmemb*size;
   }
  
  return rc;
}

wcsCurlMemoryStream::wcsCurlMemoryStream(const std::string& url)
{
   theMaxRedirects = 1;
   theFollowLocationFlag = 0;
   theCurl = curl_easy_init();
   curl_easy_setopt(theCurl, CURLOPT_WRITEFUNCTION, wcsCurlWrite);
   setUrl(url);
   theMaxRetryCount = 5;
   theTimeout = 0;
   theVerboseFlag = false;
   theStream = new wcsMemoryStream;
#ifdef WCS_DEBUG
   theVerboseFlag = true;
#endif
   
}

wcsCurlMemoryStream::~wcsCurlMemoryStream()
{
   if(theCurl)
   {
      curl_easy_cleanup(theCurl);
      theCurl = NULL;
   }
   
   theStream->clear();
}

void wcsCurlMemoryStream::setUrl(const wcsUrl& url)
{
   theUrl = url;
   curl_easy_setopt(theCurl, CURLOPT_URL, theUrl.url().c_str());
}

wcsUrl wcsCurlMemoryStream::getUrl()const
{
   return theUrl;
}

void wcsCurlMemoryStream::setFollowLocationFlag(unsigned int flag)
{
   theFollowLocationFlag = flag;
}

void wcsCurlMemoryStream::setMaxRedirects(unsigned int maxRedirects)
{
   theMaxRedirects = maxRedirects;
}

bool wcsCurlMemoryStream::download(const std::string& filename)
{
   theStream = new wcsMemoryStream;
   std::ostream* outStream = theStream.get();
   bool needToDeleteStream = false;
   bool result = false;
   wcsCurlMemoryStreamStruct outStruct;
   theStream->clear();
   if(filename != "")
   {
      std::ofstream* outFileStream = new std::ofstream;
	  //PJ//if filename is txt dont use binary mode
	  if(filename.find(".txt"))
		outFileStream->open(filename.c_str(), std::ios::out);
	  else
		outFileStream->open(filename.c_str(),
                          std::ios::out|std::ios::binary);
      if(!outFileStream->fail())
      {
         outStream = outFileStream;
         needToDeleteStream = true;
      }
      else
      {
         delete outFileStream;
         outFileStream = 0;
      }
   }
   outStruct.curlStream = outStream;
   curl_easy_setopt(theCurl, CURLOPT_FOLLOWLOCATION, theFollowLocationFlag);
   curl_easy_setopt(theCurl, CURLOPT_MAXREDIRS, theMaxRedirects);
   curl_easy_setopt(theCurl, CURLOPT_WRITEDATA, (void*)(&outStruct));
   if(!theProxyHost.empty())
   {
      if(!theProxyUser.empty())
      {
         curl_easy_setopt(theCurl, CURLOPT_PROXYUSERPWD, (theProxyUser+":"+theProxyPassword).c_str());
      }
      curl_easy_setopt(theCurl, CURLOPT_PROXY,theProxyHost.c_str());
      if(!theProxyPort.empty())
      {
         curl_easy_setopt(theCurl, CURLOPT_PROXYPORT, std::atoi(theProxyPort.c_str()));
      }
   }
  
   curl_easy_setopt(theCurl, CURLOPT_VERBOSE, true);//theVerboseFlag);
   if(theTimeout > 0)
   {
      curl_easy_setopt(theCurl, CURLOPT_TIMEOUT, theTimeout);
   }
   
   int rc = curl_easy_perform(theCurl);
   result = (rc < 1);
   
   unsigned int currentCount = 0;
   
   while(!result &&
         (currentCount < theMaxRetryCount))
   {
      wcsNotify(wcsNotifyLevel_WARN) << "wcsCurlMemoryStream::download() INFO: ***** reattempting HTTP::GET *****" << std::endl;
      rc = curl_easy_perform(theCurl);
      result = (rc < 1);
      ++currentCount;
   }

   if(!result)
   {
      wcsNotify(wcsNotifyLevel_WARN) << "wcsCurlMemoryStream::download() WARNING: Unable to get address: " << theUrl << std::endl;
   }
   
   if(needToDeleteStream)
   {
      delete outStream;
   }

   return result;
}

wcsRefPtr<wcsMemoryStream> wcsCurlMemoryStream::getStream()
{
   return theStream;
}

const wcsRefPtr<wcsMemoryStream> wcsCurlMemoryStream::getStream()const
{
   return theStream;
}

void wcsCurlMemoryStream::setTimeout(unsigned int timeout)
{
   theTimeout = timeout;
}
void wcsCurlMemoryStream::setMaxNumberRetry(unsigned int retryCount)
{
   theMaxRetryCount = retryCount;
}

void wcsCurlMemoryStream::setVerboseFlag(bool verboseFlag)
{
   theVerboseFlag = verboseFlag;
}

void wcsCurlMemoryStream::setProxyHost(const std::string& host)
{
   theProxyHost = host;
}

void wcsCurlMemoryStream::setProxyPort(const std::string& port)
{
   theProxyPort = port;
}

void wcsCurlMemoryStream::setProxyUser(const std::string& user)
{
   theProxyUser = user;
}

void wcsCurlMemoryStream::setProxyPassword(const std::string passwd)
{
   theProxyPassword = passwd;
}
   
