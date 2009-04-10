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
#ifndef wcsCapabilitiesRoot_HEADER
#define wcsCapabilitiesRoot_HEADER
#include <string>
#include <iostream>
#include <vector>
#include <wcs/wcsReferenced.h>
#include <wcs/wcsRefPtr.h>
#include <wcs/wcsCapabilitiesState.h>

class WCS_EXPORT wcsCapabilitiesRoot : public wcsReferenced
{
public:
   friend std::ostream& operator <<(std::ostream& out, const wcsCapabilitiesRoot& src);
   wcsCapabilitiesRoot();
   ~wcsCapabilitiesRoot();
   wcsRefPtr<wcsCapabilitiesState> root();
   const wcsRefPtr<wcsCapabilitiesState> root()const;
   const std::string& wcsVersion()const;
   std::string& wcsVersion();
   const std::string& wcsServerUrl()const;
   std::string& wcsServerUrl();
   wcsRefPtr<wcsCapabilitiesState> getNodeGivenName(const std::string& name);
   void getLeafNodes(std::vector<wcsRefPtr<wcsCapabilitiesState> >& leafNodes, bool includeGroupNodes=false);
   bool getLatLonBoundingBox(double& minLat,
                             double& minLon,
                             double& maxLat,
                             double& maxLon)const;
   
protected:
   wcsRefPtr<wcsCapabilitiesState> theRootLayer;
   std::string theWcsVersion;
   std::string theWcsServerUrl;
};

#endif
