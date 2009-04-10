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
#include <wcs/wcsCapabilitiesRoot.h>
#include <wcs/wcsCapabilitiesState.h>

std::ostream& operator <<(std::ostream& out, const wcsCapabilitiesRoot& src)
{
   out << "WcsVersion:    " << src.theWcsVersion << std::endl
       << "WcsServer:     " << src.theWcsServerUrl << std::endl
       << *src.root();
   
   return out;
}

wcsCapabilitiesRoot::wcsCapabilitiesRoot()
{
   theRootLayer = new wcsCapabilitiesState;
}

wcsCapabilitiesRoot::~wcsCapabilitiesRoot()
{
   theRootLayer = NULL;
}

wcsRefPtr<wcsCapabilitiesState> wcsCapabilitiesRoot::root()
{
   return theRootLayer;
}

const wcsRefPtr<wcsCapabilitiesState> wcsCapabilitiesRoot::root()const
{
   return theRootLayer;
}

const std::string& wcsCapabilitiesRoot::wcsVersion()const
{
   return theWcsVersion;
}

std::string& wcsCapabilitiesRoot::wcsVersion()
{
   return theWcsVersion;
}

const std::string& wcsCapabilitiesRoot::wcsServerUrl()const
{
   return theWcsServerUrl;
}

std::string& wcsCapabilitiesRoot::wcsServerUrl()
{
   return theWcsServerUrl;
}

wcsRefPtr<wcsCapabilitiesState> wcsCapabilitiesRoot::getNodeGivenName(const std::string& name)
{
   if(theRootLayer->name() == name)
   {
      return theRootLayer.get();
   }
   if(theRootLayer->hasChildren())
   {
      std::vector<wcsRefPtr<wcsCapabilitiesState> > stateStack;
      stateStack = theRootLayer->children();
      while(stateStack.size()>0)
      {
         wcsRefPtr<wcsCapabilitiesState> current = stateStack[stateStack.size()-1];
         stateStack.pop_back();
         if(current.valid())
         {
            if(current->name() == name)
            {
               return current.get();
            }
            if(current->hasChildren())
            {
               stateStack.insert(stateStack.end(),
                                 current->children().begin(),
                                 current->children().end());
            }
         }
      }
   }

   return 0;
}

void wcsCapabilitiesRoot::getLeafNodes(std::vector<wcsRefPtr<wcsCapabilitiesState> >& leafNodes, bool includeGroupNodes)
{
   if(!theRootLayer.valid()) return;

   if(theRootLayer->hasChildren())
   {
      std::vector<wcsRefPtr<wcsCapabilitiesState> > stateStack;
      stateStack = theRootLayer->children();
      while(stateStack.size()>0)
      {
         wcsRefPtr<wcsCapabilitiesState> current = stateStack[stateStack.size()-1];
         stateStack.pop_back();
         if(current.valid())
         {
            if(current->hasChildren())
            {
               stateStack.insert(stateStack.end(),
                                 current->children().begin(),
                                 current->children().end());
               if(includeGroupNodes&& (current.get() != root().get()))
               {
                  leafNodes.push_back(current);
               }
            }
            else
            {
               leafNodes.push_back(current);
            }
         }
      }
   }
   else
   {
      leafNodes.push_back(theRootLayer);
   }
}

bool wcsCapabilitiesRoot::getLatLonBoundingBox(double& minLat,
                                      double& minLon,
                                      double& maxLat,
                                      double& maxLon)const
{
   if(theRootLayer.valid())
   {
      return theRootLayer->getLatLonBoundingBox(minLat,
                                                minLon,
                                                maxLat,
                                                maxLon);
   }
   return false;
}

