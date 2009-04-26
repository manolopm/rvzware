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
#ifndef wcsCapabilitiesParser_HEADER
#define wcsCapabilitiesParser_HEADER
#include <iostream>
#include <string>
#include <vector>
#include <wcs/wcsReferenced.h>
#include <wcs/wcsRefPtr.h>

class wcsCapabilitiesState;
class wcsCapabilitiesRoot;
class WCS_EXPORT wcsCapabilitiesParser : public wcsReferenced
{
public:
   wcsCapabilitiesParser();
   
   wcsRefPtr<wcsCapabilitiesRoot> parse(const std::string& inputFile);
   wcsRefPtr<wcsCapabilitiesRoot> parse(std::istream& in);
   wcsRefPtr<wcsCapabilitiesRoot> parseRawString(const std::string& rawCapabilities);

   static void start(void *data, const char *element, const char **attrs);
   static void end(void *data, const char *element);
   static void handler(void *data, const char *element, int len);
   
   // Function to determine if we're inside a particular tag.  This could
   // eliminate a lot of state variables.  Returns the depth of the
   // current tag under the queried tag or 0 if not under the queried tag.
   unsigned int getTagDepth(const std::string& tag)const;
   std::vector<wcsRefPtr<wcsCapabilitiesState> >& getStateStack();
   std::vector<std::string>& getTagStack();
   wcsRefPtr<wcsCapabilitiesRoot> root();
   const wcsRefPtr<wcsCapabilitiesRoot> root()const;
   const bool& processDimSeqExtent()const;
   bool& processDimSeqExtent();
   bool& processTimeExtent();
   const bool& processTimeExtent()const;
   static std::string trim(const std::string& s);
   static void addTimes(std::vector<std::string> & times, std::string el);
   
protected:
   ~wcsCapabilitiesParser();
   
   wcsRefPtr<wcsCapabilitiesRoot> theRoot;
   std::vector<wcsRefPtr<wcsCapabilitiesState> > theStateStack;
   std::vector<std::string>  theTagStack;
   mutable bool theProcessDimSeqExtent;
   mutable bool theProcessTimeExtent;
};

#endif
