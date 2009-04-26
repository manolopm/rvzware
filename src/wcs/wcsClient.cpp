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
#include <sstream>
#include <wcs/wcsClient.h>
#include <wcs/wcsCurlMemoryStream.h>
#include <wcs/wcsUrl.h>
#include <wcs/wcsMemoryStream.h>
#include <string.h>
#include <wcs/wcs.h>
#include <iomanip>

wcsClient::wcsClient()
   :theTimeout(0),
    theMaxNumberRetry(5)
{
}

wcsClient::~wcsClient()
{
}

wcsUrl wcsClient::getMapUrl(const wcsUrl& url,
                            unsigned int width,
                            unsigned int height,
                            const double& minLat,
                            const double& minLon,
                            const double& maxLat,
                            const double& maxLon,
                            const std::string& imageFormat,
                            const std::string& version,
                            const std::string& projection)const
{
   wcsUrl tempUrl = url;
   std::stringstream s;
   std::string protocol = url.protocol();
   std::string server   = url.server();
   std::string path     = url.path();
   std::string options  = url.options();
   s << "REQUEST=GetCoverage";

   if(!version.empty())
   {
      s<<"&VERSION=" <<version;
   }
   if(!imageFormat.empty())
   {
      s << "&FORMAT=" << imageFormat;
   }
   if(!projection.empty())
   {
     s<< "&CRS=" << projection;
   }

  // s << "&WIDTH=" << width
  //   << "&HEIGHT=" << height << std::setiosflags(std::ios::fixed)
  //   << std::setprecision(15)
  //   << "&BBOX="<<minLon <<","<<minLat<<","<<maxLon<<","<<maxLat
	 //<< "&RESX=200&RESY=200"
	 //<< "&REFERER=CAPAWARE";
   
   s << std::setiosflags(std::ios::fixed)
     << std::setprecision(15)
     << "&BBOX="<<minLon <<","<<minLat<<","<<maxLon<<","<<maxLat
	 << "&RESX="<<width<<"&RESY="<<height
	 //<< "&WIDTH="<<width<<"&HEIGHT="<<height
	 << "&REFERER=CAPAWARE";

   if(options == "")
   {
      options = s.str();
   }
   else if( (*(--options.end())) != '&')
   {
      options += "&";
   }
   options += s.str();
   tempUrl.mergeUrl("http", // protocol,
                    server,
                    path,
                    options);

   return tempUrl;
};

bool wcsClient::getMap(const wcsUrl& url,
                       unsigned int width,
                       unsigned int height,
                       const double& minLat,
                       const double& minLon,
                       const double& maxLat,
                       const double& maxLon,
                       const std::string& imageFormat,
                       const std::string& version,
                       const std::string& filename,
                       const std::string& projection)const
{
   wcsUrl tempUrl = getMapUrl(url,
                              width,
                              height,
                              minLat,
                              minLon,
                              maxLat,
                              maxLon,
                              imageFormat,
                              version,
                              projection);

   return get(tempUrl, filename);
}


bool wcsClient::getCapabilities(const wcsUrl& url,
                                const std::string filename)const
{
   wcsUrl tempUrl = getCapabilitiesUrl(url);
   if(!tempUrl.url().empty())
   {
      return get(tempUrl, filename);
   }
   
   return false;
}

wcsUrl wcsClient::getCapabilitiesUrl(const wcsUrl& url)const
{
   wcsUrl result;
   if(url!= "")
   {
      std::string optionStr = url.options();
      std::string request = url.options();
      optionStr = wcsUpcase(optionStr);
      if (!wcsStringContains(optionStr, "REQUEST"))
      {
         if(optionStr == "")
         {
            request = "REQUEST=GetCapabilities";
         }
         else
         {
            request = request+"&REQUEST=GetCapabilities";
         }
      }
      if(!wcsStringContains(optionStr,"VERSION"))
      {
        request = request + "&VERSION=1.1";
      }
      if(!wcsStringContains(optionStr, "SERVICE"))
      {
        request = request + "&SERVICE=WCS";
      }

	  result.mergeUrl("http",
                      url.server(),
                      url.path(),
                      request);
   }

   return result;
}

bool wcsClient::getDescribeCoverage(const wcsUrl& url,
                                const std::string filename)const
{
   wcsUrl tempUrl = getDescribeCoverageUrl(url);
   if(!tempUrl.url().empty())
   {
      return get(tempUrl, filename);
   }
   
   return false;
}

wcsUrl wcsClient::getDescribeCoverageUrl(const wcsUrl& url)const
{
   wcsUrl result;
   if(url!= "")
   {
      std::string optionStr = url.options();
      std::string request = url.options();
      optionStr = wcsUpcase(optionStr);
      if (!wcsStringContains(optionStr, "REQUEST"))
      {
         if(optionStr == "")
         {
            request = "REQUEST=DescribeCoverage";
         }
         else
         {
            request = request+"&REQUEST=DescribeCoverage";
         }
      }
      if(!wcsStringContains(optionStr,"VERSION"))
      {
        request = request + "&VERSION=1.1";
      }
      if(!wcsStringContains(optionStr, "SERVICE"))
      {
        request = request + "&SERVICE=WCS";
      }

	  result.mergeUrl("http",
                      url.server(),
                      url.path(),
                      request);
   }

   return result;
}


bool wcsClient::get(const wcsUrl& url,
                    const std::string& filename)const
{
   bool result = false;
   theStream = 0;
   wcsRefPtr<wcsCurlMemoryStream> curlMemoryStream = new wcsCurlMemoryStream;
   
   if(url!= "")
   {
      curlMemoryStream->setUrl(url.url());
      curlMemoryStream->setTimeout(theTimeout);
      curlMemoryStream->setMaxNumberRetry(theMaxNumberRetry);
      curlMemoryStream->setProxyHost(theProxyHost);
      curlMemoryStream->setProxyPort(theProxyPort);
      curlMemoryStream->setProxyUser(theProxyUser);
      curlMemoryStream->setProxyPassword(theProxyPassword);
      result = curlMemoryStream->download(filename);
      if(result&&(filename==""))
      {
         if(curlMemoryStream->getStream().valid())
         {
            theStream =  new wcsMemoryStream(curlMemoryStream->getStream()->getBuffer(),
                                             curlMemoryStream->getStream()->getBufferSize());
            theStream->seekg(0);
            theStream->seekp(0);
         }
      }
   }

   return result;
}

wcsRefPtr<wcsMemoryStream> wcsClient::getStream()
{
   return theStream.get();
}

const wcsRefPtr<wcsMemoryStream> wcsClient::getStream()const
{
   return theStream.get();
}

void wcsClient::setMaxNumberRetry(unsigned int maxNumberRetry)
{
   theMaxNumberRetry = maxNumberRetry;
}

unsigned int wcsClient::getMaxNumberRetry()const
{
   return theMaxNumberRetry;
}


void wcsClient::setTimeout(unsigned int timeout)
{
   theTimeout = timeout;
}

unsigned int wcsClient::getTimeout()const
{
   return theTimeout;
}

void wcsClient::setProxyHost(const std::string& host)
{
   theProxyHost = host;
}

void wcsClient::setProxyPort(const std::string& port)
{
   theProxyPort = port;
}

void wcsClient::setProxyUser(const std::string& user)
{
   theProxyUser = user;
}

void wcsClient::setProxyPassword(const std::string passwd)
{
   theProxyPassword = passwd;
}
