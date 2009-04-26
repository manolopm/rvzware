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
#include <fstream>
#include <map>
#include <vector>
#include <expat.h>
#include <string.h>
#include <wcs/wcsCapabilitiesParser.h>
#include <wcs/wcsCapabilitiesState.h>
#include <wcs/wcsCapabilitiesRoot.h>
#include <wcs/wcsStyle.h>
#include <wcs/wcsMemoryStream.h>
#include <wcs/iso8601.h>

wcsCapabilitiesParser::wcsCapabilitiesParser()
{
   theRoot = NULL;
}

wcsCapabilitiesParser::~wcsCapabilitiesParser()
{
}

wcsRefPtr<wcsCapabilitiesRoot> wcsCapabilitiesParser::parse(const std::string& inputFile)
{
   std::ifstream in;

   in.open(inputFile.c_str(),
           std::ios::in|std::ios::binary);
                 
   return parse(in);
}

wcsRefPtr<wcsCapabilitiesRoot> wcsCapabilitiesParser::parse(std::istream& in)
{
   if(!in)
   {
      return 0;
   }
   theRoot = new wcsCapabilitiesRoot;
   theStateStack.push_back(theRoot->root());
   
   std::stringstream out;
   char c;
   while(!in.fail())
   {
      in.get(c);
      if(!in.fail())
      {
         out << (char)c;
      }
   }
   out << std::endl;

   std::string s = out.str();
   if(s!="")
   {
      XML_Parser p = XML_ParserCreate(NULL);
      XML_SetElementHandler(p, wcsCapabilitiesParser::start, wcsCapabilitiesParser::end);
      XML_SetCharacterDataHandler(p, wcsCapabilitiesParser::handler);
      XML_SetUserData(p, this);
      int done = 0;
      if(!XML_Parse(p, (char*)s.c_str(), s.size(), done))
      {
         theRoot = new wcsCapabilitiesRoot;
         return 0;
      }
   }
   else
   {
      theRoot = new wcsCapabilitiesRoot;
      
      return 0;
   }
   theStateStack.clear();
   
   return theRoot.get();
}

wcsRefPtr<wcsCapabilitiesRoot> wcsCapabilitiesParser::parseRawString(const std::string& rawCapabilities)
{
   std::istringstream in(rawCapabilities);

   return parse(in);
}

void printMultimap(const std::multimap<std::string, std::string>& mmap)
{
   std::multimap<std::string, std::string>::const_iterator iter = mmap.begin();
   while(iter != mmap.end())
   {
      std::cout << "<" << (*iter).first << ", " << (*iter).second << ">"<<std::endl;
      ++iter;
   }
   
}

void wcsCapabilitiesParser::start(void *data, const char *element, const char **attrs)
{
   wcsCapabilitiesParser* parse = (wcsCapabilitiesParser*)data;
   if(!parse) return;
   
   std::string el(element);

   typedef std::multimap<std::string, std::string> attr_t;
   attr_t attr;
   int i;
   for (i = 0; attrs[i]; i+=2)
   {
      attr.insert(attr_t::value_type(attrs[i], attrs[i + 1]));
   }
//   std::cout << "ATTRIBUTES for " << el << std::endl;
//   printMultimap(attr);
   std::vector<wcsRefPtr<wcsCapabilitiesState> >& stateStack = parse->getStateStack();
   std::vector<std::string>& tagStack    = parse->getTagStack();

   if(stateStack.size() < 1)
   {
      return;
   }
   wcsRefPtr<wcsCapabilitiesState> layer = stateStack[stateStack.size() - 1];

   std::vector<wcsStyle>::iterator style = layer->styles().end();
   if (layer->styles().size() > 0)
   {
      style = layer->styles().begin() + layer->styles().size() - 1;
   }
   
   if (el == "WCS_Capabilities" )
   {
      if (attr.find("version") != attr.end())
      {
         parse->root()->wcsVersion() = attr.find("version")->second;
      }
   }
   
   // Place an extra newline in the abstract when the ContactInformation
   // tag is encountered.
   if (el == "contactInfo")
   {
      layer->contactInformation() = trim(layer->contactInformation());
   }

   std::string wcsVersion = parse->root()->wcsVersion();


 //  // Layer Name
 //  if (el == "name" && parse->getTagDepth("wcs:CoverageOfferingBrief") )
 //  {
	//   std::string name ="";
 //  }

	//if (parse->getTagDepth("name"))
	//{
	//	layer->name() += el;
	//}

 //  // Layer Label
 //  if (el == "label" && parse->getTagDepth("wcs:CoverageOfferingBrief") )
 //  {
	//   layer->title();
	//   std::string name ="";
 //  }
 //  // Layer lonLat
 //  if (el == "lonLatEnvelope" && parse->getTagDepth("wcs:CoverageOfferingBrief") )
 //  {
	//   //layer->SRS() = attr.find("srsName")->second;
	//   layer->lonMin() = "0";
	//   layer->latMin() = "0";
	//   layer->lonMax() = "0";
	//   layer->latMax() = "0";
 //  }

   
  
   if (el == "LatLonBoundingBox" && parse->getTagDepth("Layer"))
   {
      layer->latMin() = attr.find("miny")->second;
      layer->latMax() = attr.find("maxy")->second;
      layer->lonMin() = attr.find("minx")->second;
      layer->lonMax() = attr.find("maxx")->second;
   }

   if (el == "BoundingBox" && parse->getTagDepth("Layer"))
   {
      attr_t::iterator it = attr.find("SRS");
      if (it == attr.end())
      {
         it = attr.find("CRS");
      }
      if (it != attr.end())
      {
		  std::vector<std::string>::iterator s_it=layer->SRS().begin();
		  while((s_it != layer->SRS().end())&&((*s_it) != (trim(it->second)))) s_it++;

		  if(s_it == layer->SRS().end())
		  {
			  layer->SRS().push_back(trim(it->second));
			 //if ((wcsVersion < "1.3.0" && it->second == "EPSG:4326") || 
			 //    it->second == "CRS:84")
			 //{
				layer->UTMlatMin() = attr.find("miny")->second;
				layer->UTMlatMax() = attr.find("maxy")->second;
				layer->UTMlonMin() = attr.find("minx")->second;
				layer->UTMlonMax() = attr.find("maxx")->second;
			 //}
				layer->UTM_vector().push_back(std::string(attr.find("miny")->second + " "+ attr.find("maxy")->second + " "+
														  attr.find("minx")->second + " "+ attr.find("maxx")->second));
		  }
      }
   }
   
   if (el == "wcs:CoverageOfferingBrief")
   {
      wcsCapabilitiesState *newLayer = new wcsCapabilitiesState;
      newLayer->inherit(*stateStack[stateStack.size()-1]);
      stateStack[stateStack.size()-1]->addChild(newLayer);
      stateStack.push_back(newLayer);
      if(attr.find("fixedWidth") != attr.end())
      {
         newLayer->fixedWidth() = atoi(attr.find("fixedWidth")->second.c_str());
      }
      if(attr.find("fixedHeight") != attr.end())
      {
         newLayer->fixedHeight() = atoi(attr.find("fixedHeight")->second.c_str());
      }
      if(attr.find("queryable") != attr.end())
      {
         newLayer->queryableFlag() = attr.find("queryable")->second;
      }
      if(attr.find("opaque") != attr.end())
      {
         newLayer->opaqueFlag() = attr.find("opaque")->second;
      }
      if(attr.find("cascaded") != attr.end())
      {
         newLayer->cascadedFlag() = attr.find("cascaded")->second;
      }
   }
   tagStack.push_back(el);
}

void wcsCapabilitiesParser::end(void *data, const char *element)
{
   wcsCapabilitiesParser* parse = (wcsCapabilitiesParser*)data;
   std::vector<wcsRefPtr<wcsCapabilitiesState> >& stateStack = parse->getStateStack();
   std::vector<std::string>& tagStack    = parse->getTagStack();
   wcsRefPtr<wcsCapabilitiesState> layer = stateStack[stateStack.size() - 1];
   if(!parse) return;
   std::string el(element);

   if(el == "ServiceExceptionReport")
   {
   }
   else if(el == "wcs:CoverageOfferingBrief")
   {
      stateStack.pop_back();
   }
   else if (el == "Extent") {
      parse->processTimeExtent()   = false;
      parse->processDimSeqExtent() = false;
   }
   else if (el == "Dimension")
   {
      parse->processTimeExtent() = false;
      parse->processDimSeqExtent() = false;
   }
   
   std::string startTag = tagStack[tagStack.size() - 1];
   if (startTag != el)
   {
      std::cerr << "End tag " << el << " does not match start tag " << startTag << ".\n";
   }

   tagStack.pop_back();
}

void wcsCapabilitiesParser::handler(void *data, const char *element, int len)
{
   wcsCapabilitiesParser* parse = (wcsCapabilitiesParser*)data;
   std::string el(element, len);

   if(parse->getStateStack().size() < 1)
   {
      // need an exception here
      return;
   }
   wcsRefPtr<wcsCapabilitiesState> layer = parse->getStateStack()[parse->getStateStack().size() - 1];
   
   if (parse->getTagDepth("Service"))
   {
      if (parse->getTagDepth("label"))
      {
         layer->title() = el;
      }
      if (parse->getTagDepth("Abstract"))
      {
         layer->abstract() += el;
      }
      if (parse->getTagDepth("ContactPerson"))
      {
         layer->contactPerson() += el;
      }
      if (parse->getTagDepth("ContactOrganization"))
      {
         layer->contactOrganization() += el ;
      }
      if (parse->getTagDepth("Address"))
      {
         layer->address() += el;
      }
      if (parse->getTagDepth("City"))
      {
         layer->city() += el;
      }
      if (parse->getTagDepth("StateOrProvince"))
      {
         layer->stateOrProvince() += el;
      }
      if (parse->getTagDepth("PostCode"))
      {
         layer->postalCode() += el;
      }
      if (parse->getTagDepth("Country"))
      {
         layer->country() += el;
      }
      if (parse->getTagDepth("ContactVoiceTelephone"))
      {
         layer->voiceTelephone() += el;
      }
      if (parse->getTagDepth("ContactFacsimileTelephone"))
      {
         layer->faxNumber() += el;
      }
      if (parse->getTagDepth("ContactElectronicMailAddress"))
      {
         layer->emailAddress() += el;
      }
      if (parse->getTagDepth("Fees"))
      {
         layer->fees() += el;
      }
      if (parse->getTagDepth("AccessConstraints"))
      {
         layer->accessContraints() += el;
      }
   }
   
   // Process tags under Layer.
   if (parse->getTagDepth("wcs:CoverageOfferingBrief"))
   {
      if (parse->getTagDepth("Style"))
      {
         std::vector<wcsStyle>& styles = layer->styles(); 
         unsigned num_styles = styles.size();
         if (parse->getTagDepth("Name"))
         {
            styles[num_styles - 1].name() += el;
         }
         else if (parse->getTagDepth("Title"))
         {
            styles[num_styles - 1].title() += el;
         }
         else if (parse->getTagDepth("Abstract"))
         {
            styles[num_styles - 1].abstract() += el;
         }
      }
      else if (parse->getTagDepth("Attribution"))
      {
         // We don't care about Title in Attribution, so we have to
         // exclude it here or else it will be used.
      }
      else
      {
         // We are now inside a Layer but not inside an Attribution or Style.
         if (parse->getTagDepth("name"))
         {
            layer->name() += el;
			layer->formats().push_back("GeoTIFF");  //PJ//TEMPORAL PARA SALIR DEL PASO
         }
         else if (parse->getTagDepth("label"))
         {
            layer->title() += el;
         }
         else if (parse->getTagDepth("Abstract"))
         {
            layer->abstract() += el;
         }
         else if(parse->getTagDepth("MinScaleDenominator"))
         {
            layer->minScaleDenominator() += el;
         }
      }
   } // end if for processing layer section
       
   if (parse->getTagDepth("Capability"))
   {
      if (parse->getTagDepth("Request"))
      {
         if (parse->getTagDepth("Map") || parse->getTagDepth("GetMap"))
         {
            if (parse->getTagDepth("Format"))
            {
               layer->formats().push_back(trim(el));
            }
         }
      }
   }
   // Add all of the times singly to the times array even if there is
   // an indicated period.  It could be complicated.
   if ((parse->getTagDepth("Extent") || parse->getTagDepth("Dimension")) && 
       parse->processTimeExtent()) 
   {
      // If we got our times from the parent, we need to remove them
      // since the WCS spec says that child values replace the parent 
      // values.
      if (layer->inheritedTimes())
      {
         layer->times().clear();
         layer->inheritedTimes() = false;
      }
      parse->addTimes(layer->times(), el);
   }
   if ((parse->getTagDepth("Extent") || parse->getTagDepth("Dimension")) &&
       parse->processDimSeqExtent())
   {
      std::string::size_type a = 0, b = 0;
      a = el.find_first_not_of("/", b);
      b = el.find_first_of("/", a);
      layer->dimSeqBegin() = el.substr(a, b-a);
      a = el.find_first_not_of("/", b);
      b = el.find_first_of("/", a);
      layer->dimSeqEnd() = el.substr(a, b-a);
      a = el.find_first_not_of("/", b);
      b = el.find_first_of("/", a);
      layer->dimSeqStep() = el.substr(a, b-a);
   }
}

unsigned int wcsCapabilitiesParser::getTagDepth(const std::string& tag)const
{
   if(theTagStack.size() < 1) return 0;
   
   for (int i = ((int) theTagStack.size()) - 1; i >= 0; i--)
   {
      if (theTagStack[i] == tag)
      {
         return theTagStack.size() - (unsigned int) i;
      }
   }
   
   return 0;
}

std::vector<wcsRefPtr<wcsCapabilitiesState> >& wcsCapabilitiesParser::getStateStack()
{
   return theStateStack;
}

std::vector<std::string>& wcsCapabilitiesParser::getTagStack()
{
   return theTagStack;
}

std::string wcsCapabilitiesParser::trim(const std::string& s)
{
   int firstpos = s.find_first_not_of("\t\n\r ");
   int lastpos = s.find_last_not_of("\t\n\r ");
   std::string result;
   
   if (firstpos >= 0 && lastpos >= 0)
   {
      result = std::string(s, firstpos, lastpos - firstpos + 1);
   }
   
   return result;
   
}

wcsRefPtr<wcsCapabilitiesRoot> wcsCapabilitiesParser::root()
{
   return theRoot;
}

const wcsRefPtr<wcsCapabilitiesRoot> wcsCapabilitiesParser::root()const
{
   return theRoot;
}

const bool& wcsCapabilitiesParser::processDimSeqExtent()const
{
   return theProcessDimSeqExtent;
}

bool& wcsCapabilitiesParser::processDimSeqExtent()
{
   return theProcessDimSeqExtent;
}

bool& wcsCapabilitiesParser::processTimeExtent()
{
   return theProcessTimeExtent;
}

const bool& wcsCapabilitiesParser::processTimeExtent()const
{
   return theProcessTimeExtent;
}

void wcsCapabilitiesParser::addTimes(std::vector<std::string> & times, std::string el)
{
   // Separate element by commas
   const char *cp = el.c_str();
   while (cp && *cp)
   {
      int loc = strcspn(cp, " \t\r\n,");
      std::string sub(cp, loc);

      // sub now contains a single time or a periodic interval.  We
      // don't do well with time fragments.
      
      if (strchr(sub.c_str(), '/'))
      {
         // We have a period.  Get the three components: start, end, period
         
         wcsMyTime start_time = { -1, -1, -1, -1, -1, -1 };
         wcsMyTime end_time = { -1, -1, -1, -1, -1, -1 };
         wcsMyTime period = { 0, 0, 0, 0, 0, 0 };
         wcsMyTime nullPeriod = { 0, 0, 0, 0, 0, 0 };
        
         const char *comp = sub.c_str();
         int comploc;
         
         comploc = strcspn(comp, "/");
         std::string start_time_str(comp, comploc);
         comp += comploc + 1;
         
         comploc = strcspn(comp, "/");
         std::string end_time_str(comp, comploc);
         comp += comploc + 1;
         
         std::string period_str(comp);

         wcs_parse_time(start_time, start_time_str);
         wcs_parse_time(end_time, end_time_str);
         wcs_parse_period(period, period_str);

         if(period == nullPeriod)
         {
            times.push_back(wcs_iso8601_time(start_time));
            times.push_back(wcs_iso8601_time(end_time));
         }
         else
         {
            while (start_time < end_time)
            {
               times.push_back(wcs_iso8601_time(start_time));
               start_time = start_time + period;
            }
            times.push_back(wcs_iso8601_time(end_time));
         }
      }
      else
      {
         // Whew.  We have a single time.  This is easy.
         times.push_back(wcs_iso8601_time(sub));
      }
      
      cp += loc;
      cp += strspn(cp, " \t\r\n,");
   }
}

