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
#ifndef wcsCurlMemoryStream_HEADER
#define wcsCurlMemoryStream_HEADER
#include <wcs/wcsMemoryStream.h>
#include <wcs/wcsUrl.h>
#include <wcs/wcsReferenced.h>
#include <wcs/wcsRefPtr.h>
#include <string>

#include <curl/curl.h>

class WCS_EXPORT wcsCurlMemoryStream : public wcsReferenced
{
public:
   wcsCurlMemoryStream(const std::string& url="");
   ~wcsCurlMemoryStream();
   bool download(const std::string& filename="");
   void setUrl(const wcsUrl& url);
   wcsUrl getUrl()const;
   
   wcsRefPtr<wcsMemoryStream> getStream();
   const wcsRefPtr<wcsMemoryStream> getStream()const;
   void setTimeout(unsigned int timeout);
   void setMaxNumberRetry(unsigned int retryCount);
   void setVerboseFlag(bool verboseFlag);
   void setFollowLocationFlag(unsigned int flag);
   void setMaxRedirects(unsigned int maxRedirects);
   void setProxyHost(const std::string& host);
   void setProxyPort(const std::string& port);
   void setProxyUser(const std::string& user);
   void setProxyPassword(const std::string passwd);
   
protected:
   wcsRefPtr<wcsMemoryStream> theStream;
   wcsUrl theUrl;
   CURL* theCurl;
   unsigned int theMaxRetryCount;
   unsigned int theTimeout;
   bool theVerboseFlag;
   int theFollowLocationFlag;
   int theMaxRedirects;
   std::string theProxyHost;
   std::string theProxyPort;
   std::string theProxyUser;
   std::string theProxyPassword;
};

#endif
