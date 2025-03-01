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
#ifndef wcsClient_HEADER
#define wcsClient_HEADER
#include <string>
#include <wcs/wcsRefPtr.h>
#include <wcs/wcsReferenced.h>
#include <wcs/wcsUrl.h>
#include <wcs/wcsMemoryStream.h>


class WCS_EXPORT wcsClient : public wcsReferenced
{
public:
   wcsClient();
   ~wcsClient();
   
   bool getCapabilities(const wcsUrl& url,
                        const std::string filename="")const;
   wcsUrl getCapabilitiesUrl(const wcsUrl& url)const;
   
   wcsUrl getMapUrl(const wcsUrl& url,
                    unsigned int width,
                    unsigned int height,
                    const double& minLat,
                    const double& minLon,
                    const double& maxLat,
                    const double& maxLon,
                    const std::string& imageType="",
                    const std::string& version="",
                    const std::string& projection="")const;//"SRS=EPSG:4326")const;
   bool getMap(const wcsUrl& url,
               unsigned int width,
               unsigned int height,
               const double& minLat,
               const double& minLon,
               const double& maxLat,
               const double& maxLon,
               const std::string& imageType="",
               const std::string& version="",
               const std::string& outputFile="",
               const std::string& projection="")const;

   wcsUrl getDescribeCoverageUrl(const wcsUrl& url)const;

   bool getDescribeCoverage(const wcsUrl& url,
                            const std::string filename="")const;

   bool get(const wcsUrl& url,
            const std::string& filename="")const;

   wcsRefPtr<wcsMemoryStream> getStream();
   const wcsRefPtr<wcsMemoryStream> getStream()const;

   void setMaxNumberRetry(unsigned int maxNumberRetry);
   unsigned int getMaxNumberRetry()const;
   void setTimeout(unsigned int timeOut);
   unsigned int getTimeout()const;
   void setProxyHost(const std::string& host);
   void setProxyPort(const std::string& port);
   void setProxyUser(const std::string& user);
   void setProxyPassword(const std::string passwd);
   const std::string& proxyHost()const
      {
         return theProxyHost;
      }
   const std::string& proxyPort()const
      {
         return theProxyPort;
      }
   const std::string& proxyUser()const
      {
         return theProxyUser;
      }
   const std::string& proxyPassword()const
      {
         return theProxyPassword;
      }
protected:
   std::string theProxyHost;
   std::string theProxyPort;
   std::string theProxyUser;
   std::string theProxyPassword;
   unsigned int theTimeout;
   unsigned int theMaxNumberRetry;
   mutable wcsRefPtr<wcsMemoryStream> theStream;
};

#endif
