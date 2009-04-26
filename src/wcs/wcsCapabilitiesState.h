/* -*-c++-*- libwms - Copyright (C) since 2004
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

#ifndef wcsCapabilititesState_HEADER
#define wcsCapabilititesState_HEADER
#include <wcs/wcsStyle.h>
#include <iostream>
#include <string>
#include <wcs/wcsReferenced.h>
#include <wcs/wcsRefPtr.h>
#include <wcs/wcsExport.h>

class WCS_EXPORT wcsCapabilitiesState : public wcsReferenced
{
public:
   friend WCS_EXPORT std::ostream& operator <<(std::ostream& out, const wcsCapabilitiesState& src);
   
   wcsCapabilitiesState();

   const std::string& name()const
      {
         return theName;
      }
   std::string& name()
      {
         return theName;
      }
   std::string& title()
      {
         return theTitle;
      }
   const std::string& title()const
      {
         return theTitle;
      }
   std::string& abstract()
      {
         return theAbstract;
      }
   const std::string& abstract()const
      {
         return theAbstract;
      }
   std::string& contactPerson()
   {
      return theContactPerson;
   }
   const std::string& contactPerson()const
   {
      return theContactPerson;
   }
   std::string& contactOrganization()
   {
      return theContactOrganization;
   }
   const std::string& contactOrganization()const
   {
      return theContactOrganization;
   }
   std::string& contactInformation()
   {
      return theContactInformation;
   }
   const std::string& contactInformation()const
   {
      return theContactInformation;
   }
   std::string& address()
   {
      return theAddress;
   }
   const std::string& address()const
   {
      return theAddress;
   }
   std::string& city()
   {
      return theCity;
   }
   const std::string& city()const
   {
      return theCity;
   }
   std::string& stateOrProvince()
   {
      return theStateOrProvince;
   }
   const std::string& stateOrProvince()const
   {
      return theStateOrProvince;
   }
   std::string& postalCode()
   {
      return thePostalCode;
   }
   const std::string& postalCode()const
   {
      return thePostalCode;
   }
   std::string& country()
   {
      return theCountry;
   }
   const std::string& country()const
   {
      return theCountry;
   }
   std::string& voiceTelephone()
   {
      return theVoiceTelephone;
   }
   const std::string& voiceTelephone()const
   {
      return theVoiceTelephone;
   }
   std::string& faxNumber()
   {
      return theFaxNumber;
   }
   const std::string& faxNumber()const
   {
      return theFaxNumber;
   }
   std::string& emailAddress()
   {
      return theEmailAddress;
   }
   const std::string& emailAddress()const
   {
      return theEmailAddress;
   }
   std::string& fees()
   {
      return theFees;
   }
   const std::string& fees()const
   {
      return theFees;
   }
   std::string& accessContraints()
   {
      return theAccessConstraints;
   }
   const std::string& accessContraints()const
   {
      return theAccessConstraints;
   }
   wcsStyle getStyle(unsigned int idx)const
      {
         if(idx < theStyles.size())
         {
            return theStyles[idx];
         }
         return wcsStyle();
      }
   void addStyle(const wcsStyle& style)
      {
         theStyles.push_back(style);
      }
   void setStyle(unsigned int idx,
                 const wcsStyle& style)
      {
         if(idx < theStyles.size())
         {
            theStyles[idx] = style;
         }
      }
   unsigned int getNumberOfStyles()const
      {
         return theStyles.size();
      }
   const std::string& latMin()const
      {
         return theLatMin;
      }
   std::string& latMin()
      {
         return theLatMin;
      }
   
   const std::string& latMax()const
      {
         return theLatMax;
      }
   std::string& latMax()
      {
         return theLatMax;
      }
   
   std::string& lonMin()
      {
         return theLonMin;
      }
   const std::string& lonMin()const
      {
         return theLonMin;
      }
   std::string& lonMax()
      {
         return theLonMax;
      }
   const std::string& lonMax()const
      {
         return theLonMax;
      }
//
   const std::string& UTMlatMin()const
      {
         return theUTMLatMin;
      }
   std::string& UTMlatMin()
      {
         return theUTMLatMin;
      }
   
   const std::string& UTMlatMax()const
      {
         return theUTMLatMax;
      }
   std::string& UTMlatMax()
      {
         return theUTMLatMax;
      }
   
   std::string& UTMlonMin()
      {
         return theUTMLonMin;
      }
   const std::string& UTMlonMin()const
      {
         return theUTMLonMin;
      }
   std::string& UTMlonMax()
      {
         return theUTMLonMax;
      }
   const std::string& UTMlonMax()const
      {
         return theUTMLonMax;
      }
//
   std::vector<std::string>& times()
      {
         return theTimes;
      }
   const std::vector<std::string>& times()const
      {
         return theTimes;
      }
   std::string getTime(unsigned long idx)const
      {
         if(idx < theTimes.size())
         {
            return theTimes[idx];
         }
         return "";
      }
   void addTime(const std::string& time)
      {
         theTimes.push_back(time);
      }
   void setTime(unsigned int idx,
                const std::string& time)
      {
         theTimes[idx] = time;
      }
   std::string& timeDefault()
      {
         return theTimeDefault;
      }
   const std::string& timeDefault()const
      {
         return theTimeDefault;
      }
   std::string& dimSeqBegin()
      {
         return theDimSeqBegin;
      }
   const std::string& dimSeqBegin()const
      {
         return theDimSeqBegin;
      }
   std::string& dimSeqEnd()
      {
         return theDimSeqEnd;
      }
   const std::string& dimSeqEnd()const
      {
         return theDimSeqEnd;
      }
   std::string& dimSeqStep()
      {
         return theDimSeqStep;
      }
   const std::string& dimSeqStep()const
      {
         return theDimSeqStep;
      }
   std::string& dimSeqDefault()
      {
         return theDimSeqDefault;
      }
   const std::string& dimSeqDefault()const
      {
         return theDimSeqDefault;
      }
   bool& noSubsets()
      {
         return theNoSubsets;
      }
   const bool& noSubsets()const
      {
         return theNoSubsets;
      }
   int getFixedWidth()const
      {
         return theFixedWidth;
      }
   int& fixedWidth()
      {
         return theFixedWidth;
      }
   const int& fixedWidth()const
      {
         return theFixedWidth;
      }
   int& fixedHeight()
      {
         return theFixedHeight;
      }
   const int& fixedHeight()const
      {
         return theFixedHeight;
      }
   std::string getFormat(unsigned int idx)const
      {
         if(idx < theFormats.size())
         {
            return theFormats[idx];
         }
         return "";
      }
   std::vector<std::string>& formats()
      {
         return theFormats;
      }
   const std::vector<std::string>& formats()const
      {
         return theFormats;
      }

   //PJ
   std::string getSRS(unsigned int idx)const
      {
         if(idx < theSRS.size())
         {
            return theSRS[idx];
         }
         return "";
      }
   std::vector<std::string>& SRS()
      {
         return theSRS;
      }
   const std::vector<std::string>& SRS()const
      {
         return theSRS;
      }
   unsigned int getNumberOfSRS()const
      {
         return theSRS.size();
      }

   std::vector<std::string>& UTM_vector()
      {
         return theUTMvector;
      }
   const std::vector<std::string>& UTM_vector()const
      {
         return theUTMvector;
      }
   //end PJ

   unsigned int getNumberOfFormats()const
      {
         return theFormats.size();
      }
   bool& inheritedTimes()
      {
         return theInheritedTimes;
      }
   const bool& inheritedTimes()const
      {
         return theInheritedTimes;
      }
   bool hasChildren()const
      {
         return (theChildren.size() > 0);
      }
   wcsRefPtr<wcsCapabilitiesState> child(unsigned int idx);
   const wcsRefPtr<wcsCapabilitiesState> child(unsigned int idx)const;
   void addChild(wcsCapabilitiesState* child);
   const std::vector<wcsRefPtr<wcsCapabilitiesState> >& children()const
   {
      return theChildren;
   }
   std::vector<wcsRefPtr<wcsCapabilitiesState> >& children()
   {
      return theChildren;
   }
   
   bool& isSequence()
      {
         return theIsSequence;
      }

   const bool& isSequence()const
      {
         return theIsSequence;
      }

   std::vector<wcsStyle>& styles()
      {
         return theStyles;
      }
   const std::vector<wcsStyle>& styles()const
      {
         return theStyles;
      }
   std::string& minScaleHint()
   {
      return theMinScaleHint;
   }
   const std::string& minScaleHint()const
   {
      return theMinScaleHint;
   }
   std::string& maxScaleHint()
   {
      return theMaxScaleHint;
   }
   const std::string& maxScaleHint()const
   {
      return theMaxScaleHint;
   }
   std::string& minScaleDenominator()
   {
      return theMinScaleDenominator;
   }
   const std::string& minScaleDenominator()const
   {
      return theMinScaleDenominator;
   }
   bool hasMinMaxScaleHints()const
   {
      return ((theMinScaleHint!="")&&
              (theMaxScaleHint!=""));
   }
   bool hasMinScaleDenominator()const
   {
      return (theMinScaleDenominator!="");
   }
   std::string& opaqueFlag()
   {
      return theOpaqueFlag;
   }
   const std::string& opaqueFlag()const
   {
      return theOpaqueFlag;
   }
   std::string& queryableFlag()
   {
      return theQueryableFlag;
   }
   const std::string& queryableFlag()const
   {
      return theQueryableFlag;
   }
   std::string& cascadedFlag()
   {
      return theCascadedFlag;
   }
   const std::string& cascadedFlag()const
   {
      return theCascadedFlag;
   }
   void inherit(const wcsCapabilitiesState& src);

   bool getLatLonBoundingBox(double& minLat,
                             double& minLon,
                             double& maxLat,
                             double& maxLon)const;
   bool getUTMLatLonBoundingBox(double& minLat,
                             double& minLon,
                             double& maxLat,
                             double& maxLon)const;
protected:
   std::string theName;   /* not inherited */
   std::string theTitle;  /* not inherited */
   std::string theAbstract;    /* not inherited */
   std::string theContactPerson;
   std::string theContactOrganization;
   std::string theContactInformation;
   std::string theAddress;
   std::string theCity;
   std::string theStateOrProvince;
   std::string thePostalCode;
   std::string theCountry;
   std::string theVoiceTelephone;
   std::string theFaxNumber;
   std::string theEmailAddress;
   std::string theFees;
   std::string theAccessConstraints;
   
   std::vector<wcsStyle> theStyles; /* added to values from parent */
   std::string theLatMin;
   std::string theLatMax;
   std::string theLonMin;
   std::string theLonMax;
//
   std::vector<std::string> theSRS;  //PJ
   std::vector<std::string> theUTMvector;  //PJ
   std::string theUTMLatMin;
   std::string theUTMLatMax;
   std::string theUTMLonMin;
   std::string theUTMLonMax;
//
   std::vector<std::string> theTimes; /* from time dimension */
   std::string theTimeDefault; /* default attr in time dimension */
   std::string theDimSeqBegin; /* from dim_sequence dimension */
   std::string theDimSeqEnd;
   std::string theDimSeqStep;
   std::string theDimSeqDefault;
   std::string theMinScaleHint;
   std::string theMaxScaleHint;
   std::string theMinScaleDenominator;
   std::string theQueryableFlag;
   std::string theOpaqueFlag;
   std::string theCascadedFlag;
   
   bool theNoSubsets;
   int theFixedWidth;
   int theFixedHeight;
   
   std::vector<std::string> theFormats; // Initial values taken from the 

   bool theInheritedTimes;
   bool theIsSequence;

   std::vector<wcsRefPtr<wcsCapabilitiesState> > theChildren;
   
private:
   wcsCapabilitiesState(const wcsCapabilitiesState& src);
   ~wcsCapabilitiesState();

   
};

#endif
